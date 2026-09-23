from __future__ import annotations

import re
from dataclasses import dataclass
from pathlib import Path

from algoviz.discovery import _header_before, _mask_comments_and_literals
from algoviz.errors import AlgoVizError
from algoviz.models import FunctionDefinition


TRACE_HEADER_NAME = "algoviz_trace.h"

_DECLARATION_PATTERN = re.compile(
    r"^(?:const\s+)?(?:[A-Za-z_]\w*::)*[A-Za-z_]\w*"
    r"(?:\s*<[^;=()]+>)?(?:\s*[*&]+\s*|\s+)([A-Za-z_]\w*)\s*(?:[({=;])"
)
_CONTROL_WORDS = {"break", "case", "continue", "else", "if", "return", "switch", "while"}
_SOURCE_SCOPE_PATTERN = re.compile(
    r"(?:^|\s)(?P<kind>class|struct|namespace)\s+"
    r"(?P<name>[A-Za-z_]\w*)[^{};]*$",
    re.DOTALL,
)
_STATE_DECLARATION_PATTERN = re.compile(
    r"^(?:(?:const|constinit|constexpr|extern|inline|mutable|static|thread_local|volatile)\s+)*"
    r"(?:typename\s+)?(?:[A-Za-z_]\w*::)*[A-Za-z_]\w*"
    r"(?:\s*<[^;=()]+>)?(?:\s*[*&]+\s*|\s+)([A-Za-z_]\w*)\s*"
    r"(?:\[[^\]]*\]\s*)?(?:[={{;])"
)
_USING_MEMBER_PATTERN = re.compile(
    r"^using\s+(?:typename\s+)?(?:[A-Za-z_]\w*(?:\s*<[^;]+>)?::)+"
    r"([A-Za-z_]\w*)\s*;"
)


@dataclass(slots=True)
class _SourceScope:
    kind: str
    name: str | None
    opening: int
    closing: int = -1


def _source_scopes(masked: str) -> list[_SourceScope]:
    scopes: list[_SourceScope] = []
    stack: list[_SourceScope] = []
    for index, character in enumerate(masked):
        if character == "{":
            header, _ = _header_before(masked, index)
            match = _SOURCE_SCOPE_PATTERN.search(header)
            scope = _SourceScope(
                kind=match.group("kind") if match else "other",
                name=match.group("name") if match else None,
                opening=index,
            )
            scopes.append(scope)
            stack.append(scope)
        elif character == "}" and stack:
            stack.pop().closing = index
    return [scope for scope in scopes if scope.closing >= 0]


def _active_scopes(scopes: list[_SourceScope], offset: int) -> list[_SourceScope]:
    return [
        scope
        for scope in scopes
        if scope.opening < offset < scope.closing
    ]


def _line_offsets(source: str) -> list[int]:
    offsets = [0]
    for match in re.finditer("\n", source):
        offsets.append(match.end())
    return offsets


def _state_declaration(line: str) -> tuple[str, bool] | None:
    stripped = re.sub(
        r"^(?:public|protected|private)\s*:\s*", "", line.strip()
    )
    if not stripped or stripped.startswith(
        ("#", "class ", "enum ", "namespace ", "struct ", "template ", "typedef ")
    ):
        return None
    using_match = _USING_MEMBER_PATTERN.match(stripped)
    if using_match:
        return using_match.group(1), True
    if stripped.startswith("using ") or "(" in stripped:
        return None
    declaration_match = _STATE_DECLARATION_PATTERN.match(stripped)
    if declaration_match:
        return declaration_match.group(1), False
    return None


def _merged_variables(scopes: list[dict[str, str]]) -> dict[str, str]:
    variables: dict[str, str] = {}
    for scope in scopes:
        variables.update(scope)
    return variables


def _state_variables(
    masked: str,
    masked_lines: list[str],
    line_offsets: list[int],
    source_scopes: list[_SourceScope],
    opening: int,
    closing: int,
) -> dict[str, str]:
    variables: dict[str, str] = {}
    for line_index, line in enumerate(masked_lines):
        offset = line_offsets[line_index] + len(line) - len(line.lstrip())
        if offset >= opening:
            break
        active = _active_scopes(source_scopes, offset)
        if any(scope.kind != "namespace" for scope in active):
            continue
        declaration = _state_declaration(line)
        if declaration is None or declaration[1]:
            continue
        name = declaration[0]
        namespaces = [scope.name for scope in active if scope.name]
        qualified_name = "::".join((*namespaces, name))
        label = qualified_name if namespaces else f"::{name}"
        variables[label] = f"::{qualified_name}"

    containing_classes = [
        scope
        for scope in _active_scopes(source_scopes, opening)
        if scope.kind in {"class", "struct"}
    ]
    header, _ = _header_before(masked, opening)
    if not containing_classes or re.search(r"\bstatic\b", header):
        return variables

    containing_class = containing_classes[-1]
    body = masked[opening + 1 : closing]
    for line_index, line in enumerate(masked_lines):
        offset = line_offsets[line_index] + len(line) - len(line.lstrip())
        active = _active_scopes(source_scopes, offset)
        if not active or active[-1] is not containing_class:
            continue
        declaration = _state_declaration(line)
        if declaration is None:
            continue
        name, is_using = declaration
        if is_using and not re.search(rf"\b{re.escape(name)}\b(?!\s*\()", body):
            continue
        expression = f"this->{name}"
        variables[expression] = expression
    return variables


def _split_parameters(parameters: str) -> list[str]:
    parts: list[str] = []
    start = 0
    depth = 0
    for index, character in enumerate(parameters):
        if character in "<([{":
            depth += 1
        elif character in ">)]}":
            depth -= 1
        elif character == "," and depth == 0:
            parts.append(parameters[start:index])
            start = index + 1
    parts.append(parameters[start:])
    return parts


def _parameter_names(signature: str) -> list[str]:
    opening = signature.find("(")
    closing = signature.rfind(")")
    if opening < 0 or closing < opening:
        return []

    names: list[str] = []
    for parameter in _split_parameters(signature[opening + 1 : closing]):
        declaration = parameter.split("=", 1)[0].strip()
        identifiers = re.findall(r"[A-Za-z_]\w*", declaration)
        if identifiers and identifiers[-1] != "void":
            names.append(identifiers[-1])
    return names


def _definition_location(
    source: str, masked: str, definition: FunctionDefinition
) -> tuple[int, int, int, int]:
    offsets = [0]
    for match in re.finditer("\n", source):
        offsets.append(match.end())

    start_offset = offsets[definition.line - 1]
    opening = masked.find("{", start_offset)
    if opening < 0:
        raise AlgoVizError(
            f"Cannot find the body of '{definition.qualified_name}'."
        )

    depth = 0
    closing = -1
    for index in range(opening, len(masked)):
        if masked[index] == "{":
            depth += 1
        elif masked[index] == "}":
            depth -= 1
            if depth == 0:
                closing = index
                break
    if closing < 0:
        raise AlgoVizError(
            f"Cannot find the end of '{definition.qualified_name}'."
        )

    opening_line = source.count("\n", 0, opening)
    closing_line = source.count("\n", 0, closing)
    opening_line_start = source.rfind("\n", 0, opening) + 1
    closing_line_start = source.rfind("\n", 0, closing) + 1
    return (
        opening_line,
        opening - opening_line_start,
        closing_line,
        closing - closing_line_start,
    )


def _trace_statement(
    definition: FunctionDefinition,
    line: int,
    variables: dict[str, str],
    indentation: str,
) -> str:
    fields = "".join(
        f', ::algoviz_trace::field("{name}", {expression})'
        for name, expression in variables.items()
    )
    return (
        f'{indentation}::algoviz_trace::record("{definition.qualified_name}", '
        f"{line}{fields});\n"
    )


def _local_declaration(line: str) -> str | None:
    stripped = line.strip()
    for_match = re.match(
        r"for\s*\(\s*(?:const\s+)?(?:[A-Za-z_]\w*::)*[A-Za-z_]\w*"
        r"(?:\s*<[^;=()]+>)?(?:\s*[*&]+\s*|\s+)([A-Za-z_]\w*)\s*[({=;]",
        stripped,
    )
    if for_match:
        return for_match.group(1)
    first_word = stripped.split(None, 1)[0] if stripped else ""
    if first_word in _CONTROL_WORDS:
        return None
    match = _DECLARATION_PATTERN.match(stripped)
    return match.group(1) if match else None


def instrument_source(path: Path, definitions: list[FunctionDefinition]) -> None:
    source = path.read_text(encoding="utf-8")
    masked = _mask_comments_and_literals(source)
    source_lines = source.splitlines(keepends=True)
    masked_lines = masked.splitlines(keepends=True)
    line_offsets = _line_offsets(source)
    source_scopes = _source_scopes(masked)
    before: dict[int, list[str]] = {}
    after: dict[int, list[str]] = {}

    for definition in definitions:
        opening_line, opening_column, closing_line, closing_column = (
            _definition_location(source, masked, definition)
        )
        opening = line_offsets[opening_line] + opening_column
        closing = line_offsets[closing_line] + closing_column
        variables = _state_variables(
            masked,
            masked_lines,
            line_offsets,
            source_scopes,
            opening,
            closing,
        )
        variables.update(
            {name: name for name in _parameter_names(definition.signature)}
        )
        function_indentation = re.match(r"\s*", source_lines[opening_line]).group(0)
        indentation = function_indentation + "    "
        if opening_line == closing_line:
            line = source_lines[opening_line]
            body = line[opening_column + 1 : closing_column].strip()
            body_line = f"{indentation}{body}\n" if body else ""
            source_lines[opening_line] = (
                line[: opening_column + 1]
                + "\n"
                + _trace_statement(
                    definition, opening_line + 1, variables, indentation
                )
                + body_line
                + function_indentation
                + "}"
                + line[closing_column + 1 :]
            )
            continue

        scopes: list[dict[str, str]] = [variables]
        after.setdefault(opening_line, []).append(
            _trace_statement(
                definition, opening_line + 1, scopes[0], indentation
            )
        )
        pending_scope_variables: dict[str, str] = {}

        for line_index in range(opening_line + 1, closing_line):
            original_line = source_lines[line_index]
            masked_line = masked_lines[line_index]
            stripped = masked_line.strip()
            leading_closures = len(stripped) - len(stripped.lstrip("}"))
            for _ in range(leading_closures):
                if len(scopes) > 1:
                    scopes.pop()

            variables = _merged_variables(scopes)
            line_indentation = re.match(r"\s*", original_line).group(0)
            is_brace_only = stripped in {"{", "}"}
            is_executable = bool(stripped) and not stripped.startswith(("#", "//"))
            continues_previous_statement = bool(
                re.match(r"(?:else|catch)\b", stripped)
            )

            if (
                is_executable
                and not is_brace_only
                and not continues_previous_statement
            ):
                before.setdefault(line_index, []).append(
                    _trace_statement(
                        definition, line_index + 1, variables, line_indentation
                    )
                )

            declaration = _local_declaration(masked_line)
            if stripped.startswith("for") and declaration:
                pending_scope_variables = {declaration: declaration}
            elif declaration:
                scopes[-1][declaration] = declaration

            opening_count = masked_line.count("{")
            closing_count = masked_line.count("}") - leading_closures
            for _ in range(opening_count):
                scopes.append(pending_scope_variables)
                pending_scope_variables = {}
                nested_variables = _merged_variables(scopes)
                nested_indentation = line_indentation + "    "
                after.setdefault(line_index, []).append(
                    _trace_statement(
                        definition,
                        line_index + 1,
                        nested_variables,
                        nested_indentation,
                    )
                )
            for _ in range(closing_count):
                if len(scopes) > 1:
                    scopes.pop()

    output = [
        '#pragma push_macro("private")\n',
        "#undef private\n",
        '#pragma push_macro("protected")\n',
        "#undef protected\n",
        f'#include "{TRACE_HEADER_NAME}"\n',
        '#pragma pop_macro("protected")\n',
        '#pragma pop_macro("private")\n',
    ]
    for line_index, line in enumerate(source_lines):
        output.extend(before.get(line_index, []))
        output.append(line)
        output.extend(after.get(line_index, []))
    path.write_text("".join(output), encoding="utf-8")


def write_trace_header(path: Path) -> None:
    path.write_text(
        r'''#pragma once

#include <atomic>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iterator>
#include <set>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

namespace algoviz_trace {

inline std::string quote(const std::string& value)
{
    std::ostringstream output;
    output << '"';
    for (const unsigned char character : value)
    {
        switch (character)
        {
        case '"': output << "\\\""; break;
        case '\\': output << "\\\\"; break;
        case '\n': output << "\\n"; break;
        case '\r': output << "\\r"; break;
        case '\t': output << "\\t"; break;
        default:
            if (character < 0x20)
            {
                output << "\\u" << std::hex << std::setw(4) << std::setfill('0')
                       << static_cast<int>(character);
            }
            else
            {
                output << character;
            }
        }
    }
    output << '"';
    return output.str();
}

template <typename T, typename = void>
struct is_linked_list_pointer : std::false_type {};

template <typename T>
struct is_linked_list_pointer<
    T,
    std::void_t<
        decltype(std::declval<T>().get()),
        decltype(std::declval<T>()->val),
        decltype(std::declval<T>()->next)>> : std::true_type {};

template <typename T, typename = void>
struct is_pointer_like : std::false_type {};

template <typename T>
struct is_pointer_like<
    T,
    std::void_t<
        decltype(std::declval<T>().get()),
        decltype(*std::declval<T>())>> : std::true_type {};

template <typename T, typename = void>
struct is_iterable : std::false_type {};

template <typename T>
struct is_iterable<
    T,
    std::void_t<
        decltype(std::begin(std::declval<const T&>())),
        decltype(std::end(std::declval<const T&>()))>> : std::true_type {};

template <typename T>
std::string scalar_json(const T& value)
{
    if constexpr (std::is_same_v<std::decay_t<T>, bool>)
    {
        return value ? "true" : "false";
    }
    else if constexpr (std::is_arithmetic_v<std::decay_t<T>>)
    {
        return std::to_string(value);
    }
    else if constexpr (std::is_convertible_v<T, std::string>)
    {
        return quote(static_cast<std::string>(value));
    }
    else
    {
        return quote("<object>");
    }
}

template <typename T>
std::string pointer_id(const T* pointer)
{
    if (pointer == nullptr)
    {
        return "null";
    }
    std::ostringstream output;
    output << static_cast<const void*>(pointer);
    return quote(output.str());
}

template <typename T>
std::string linked_list_json(const T& head)
{
    std::ostringstream output;
    std::set<const void*> visited;
    auto node = head;
    bool cycle = false;
    output << "{\"kind\":\"linked-list\",\"head\":"
           << pointer_id(head ? head.get() : nullptr) << ",\"nodes\":[";
    bool first = true;
    for (int count = 0; node && count < 64; ++count)
    {
        const void* identity = static_cast<const void*>(node.get());
        if (!visited.insert(identity).second)
        {
            cycle = true;
            break;
        }
        if (!first)
        {
            output << ',';
        }
        first = false;
        output << "{\"id\":" << pointer_id(node.get())
               << ",\"value\":" << scalar_json(node->val)
               << ",\"next\":"
               << pointer_id(node->next ? node->next.get() : nullptr) << '}';
        node = node->next;
    }
    if (node && visited.count(static_cast<const void*>(node.get())) != 0)
    {
        cycle = true;
    }
    output << "],\"cycle\":" << (cycle ? "true" : "false") << '}';
    return output.str();
}

template <typename T>
std::string value_json(const T& value);

template <typename T>
std::string range_json(const T& values)
{
    std::ostringstream output;
    output << '[';
    bool first = true;
    for (const auto& value : values)
    {
        if (!first)
        {
            output << ',';
        }
        first = false;
        output << value_json(value);
    }
    output << ']';
    return output.str();
}

template <typename T>
std::string value_json(const T& value)
{
    if constexpr (is_linked_list_pointer<std::decay_t<T>>::value)
    {
        return linked_list_json(value);
    }
    else if constexpr (std::is_convertible_v<T, std::string>)
    {
        return scalar_json(value);
    }
    else if constexpr (
        std::is_pointer_v<std::decay_t<T>>
        && !std::is_void_v<std::remove_pointer_t<std::decay_t<T>>>)
    {
        return value == nullptr ? "null" : value_json(*value);
    }
    else if constexpr (is_pointer_like<std::decay_t<T>>::value)
    {
        return value.get() == nullptr ? "null" : value_json(*value);
    }
    else if constexpr (is_iterable<std::decay_t<T>>::value)
    {
        return range_json(value);
    }
    else
    {
        return scalar_json(value);
    }
}

struct Field
{
    std::string name;
    std::string json;
};

template <typename T>
Field field(const char* name, const T& value)
{
    return {name, value_json(value)};
}

inline std::atomic<unsigned long long> next_step{0};

template <typename... Fields>
void record(const char* function, int line, const Fields&... fields)
{
    const char* trace_path = std::getenv("ALGOVIZ_TRACE_FILE");
    if (trace_path == nullptr || *trace_path == '\0')
    {
        return;
    }

    std::ofstream output(trace_path, std::ios::app);
    if (!output)
    {
        return;
    }
    output << "{\"step\":" << next_step.fetch_add(1)
           << ",\"function\":" << quote(function)
           << ",\"line\":" << line << ",\"variables\":{";
    std::size_t index = 0;
    ((output << (index++ == 0 ? "" : ",") << quote(fields.name) << ':'
             << fields.json), ...);
    output << "}}\n";
}

}  // namespace algoviz_trace
''',
        encoding="utf-8",
    )