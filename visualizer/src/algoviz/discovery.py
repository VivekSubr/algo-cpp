from __future__ import annotations

import re
from pathlib import Path

from algoviz.errors import SymbolResolutionError
from algoviz.models import FunctionDefinition

_CONTROL_WORDS = {
    "catch",
    "for",
    "if",
    "requires",
    "switch",
    "while",
}
_SCOPE_PATTERN = re.compile(
    r"(?:^|\s)(?P<kind>class|struct|namespace)\s+(?P<name>[A-Za-z_]\w*)[^{};]*$",
    re.DOTALL,
)
_NAME_PATTERN = re.compile(r"(~?[A-Za-z_]\w*(?:::[A-Za-z_]\w*)*)\s*$")


def _mask_comments_and_literals(source: str) -> str:
    output = list(source)
    index = 0
    state = "code"
    quote = ""

    while index < len(source):
        current = source[index]
        following = source[index + 1] if index + 1 < len(source) else ""

        if state == "code":
            if current == "/" and following == "/":
                output[index] = output[index + 1] = " "
                index += 2
                state = "line_comment"
                continue
            if current == "/" and following == "*":
                output[index] = output[index + 1] = " "
                index += 2
                state = "block_comment"
                continue
            if current in {'"', "'"}:
                quote = current
                output[index] = " "
                index += 1
                state = "literal"
                continue
        elif state == "line_comment":
            if current == "\n":
                state = "code"
            else:
                output[index] = " "
        elif state == "block_comment":
            if current == "*" and following == "/":
                output[index] = output[index + 1] = " "
                index += 2
                state = "code"
                continue
            if current != "\n":
                output[index] = " "
        elif state == "literal":
            if current == "\\":
                output[index] = " "
                if index + 1 < len(source):
                    if source[index + 1] != "\n":
                        output[index + 1] = " "
                    index += 2
                    continue
            if current == quote:
                state = "code"
            if current != "\n":
                output[index] = " "

        index += 1

    return "".join(output)


def _header_before(source: str, brace_index: int) -> tuple[str, int]:
    boundary = max(
        source.rfind(";", 0, brace_index),
        source.rfind("{", 0, brace_index),
        source.rfind("}", 0, brace_index),
    )
    start = boundary + 1
    return source[start:brace_index].strip(), start


def _matching_parenthesis(text: str, opening: int) -> int | None:
    depth = 0
    for index in range(opening, len(text)):
        if text[index] == "(":
            depth += 1
        elif text[index] == ")":
            depth -= 1
            if depth == 0:
                return index
    return None


def _function_from_header(
    source: str,
    header: str,
    header_start: int,
    scopes: tuple[str, ...],
) -> FunctionDefinition | None:
    if not header or re.search(r"\]\s*\(", header):
        return None

    opening = header.find("(")
    if opening < 0:
        return None

    closing = _matching_parenthesis(header, opening)
    if closing is None:
        return None

    name_match = _NAME_PATTERN.search(header[:opening])
    if name_match is None:
        return None

    name = name_match.group(1)
    simple_name = name.rsplit("::", 1)[-1]
    if simple_name in _CONTROL_WORDS:
        return None

    name_start = name_match.start(1)
    preceding = header[name_start - 1] if name_start else ""
    if preceding == "." or header[max(0, name_start - 2):name_start] == "->":
        return None

    trailing = header[closing + 1 :].strip()
    if trailing and not (
        trailing.startswith(":")
        or re.fullmatch(
            r"(?:const|constexpr|noexcept(?:\s*\([^)]*\))?|override|final|"
            r"volatile|&|&&|\[\[[^\]]+\]\]|->\s*[\w:<>,*&\s]+|\s)+",
            trailing,
        )
    ):
        return None

    if "::" in name:
        qualified_name = name
    elif scopes:
        qualified_name = "::".join((*scopes, name))
    else:
        qualified_name = name

    absolute_name_start = header_start + header.find(name, max(0, opening - len(name) - 1))
    line = source.count("\n", 0, absolute_name_start) + 1
    signature = " ".join(header[name_start : closing + 1].split())
    return FunctionDefinition(
        name=simple_name,
        qualified_name=qualified_name,
        signature=signature,
        line=line,
    )


class BootstrapFunctionDiscovery:
    """Dependency-free bootstrap provider; Clang will replace this parser."""

    def discover(self, path: Path) -> list[FunctionDefinition]:
        source = path.read_text(encoding="utf-8")
        masked = _mask_comments_and_literals(source)
        stack: list[tuple[str, str | None]] = []
        scopes: list[str] = []
        definitions: list[FunctionDefinition] = []

        for index, character in enumerate(masked):
            if character == "{":
                header, header_start = _header_before(masked, index)
                scope_match = _SCOPE_PATTERN.search(header)
                if scope_match:
                    name = scope_match.group("name")
                    stack.append(("scope", name))
                    scopes.append(name)
                    continue

                definition = _function_from_header(
                    source,
                    header,
                    header_start,
                    tuple(scopes),
                )
                if definition is not None:
                    definitions.append(definition)
                    stack.append(("function", None))
                else:
                    stack.append(("block", None))
            elif character == "}" and stack:
                kind, name = stack.pop()
                if kind == "scope" and name is not None:
                    scopes.pop()

        return definitions


def select_definitions(
    definitions: list[FunctionDefinition],
    requested: str | None,
) -> list[FunctionDefinition]:
    if requested is None:
        return definitions

    matches = [
        definition
        for definition in definitions
        if requested in {definition.name, definition.qualified_name}
    ]
    if not matches:
        available = ", ".join(
            sorted({definition.qualified_name for definition in definitions})
        )
        detail = f" Available definitions: {available}." if available else ""
        raise SymbolResolutionError(
            f"Function '{requested}' is not defined in the requested file.{detail}"
        )
    if len(matches) > 1:
        signatures = ", ".join(
            f"{definition.qualified_name}: {definition.signature}"
            for definition in matches
        )
        raise SymbolResolutionError(
            f"Function '{requested}' is ambiguous. Choose a signature: {signatures}"
        )
    return matches
