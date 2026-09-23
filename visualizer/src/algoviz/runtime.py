from __future__ import annotations

import json
import os
import shutil
import subprocess
from dataclasses import dataclass
from pathlib import Path

from algoviz.discovery import BootstrapFunctionDiscovery
from algoviz.errors import AlgoVizError
from algoviz.instrumentation import (
    TRACE_HEADER_NAME,
    instrument_source,
    write_trace_header,
)
from algoviz.workspace import RunWorkspace


_IGNORED_DIRECTORIES = {
    ".algoviz",
    ".git",
    ".pytest_cache",
    "__pycache__",
    "build",
}


@dataclass(frozen=True, slots=True)
class RuntimeResult:
    status: str
    exit_code: int | None
    stdout: str
    stderr: str
    trace_path: Path
    events: list[dict[str, object]]
    test_command: list[str]


def find_cmake_project(input_path: Path, source_root: Path) -> Path:
    for candidate in (input_path.parent, *input_path.parents):
        if (candidate / "CMakeLists.txt").is_file():
            return candidate
        if candidate == source_root:
            break
    raise AlgoVizError(
        f"No CMakeLists.txt was found above '{input_path}' within the repository."
    )


def _ignore_copy(directory: str, names: list[str]) -> set[str]:
    del directory
    return {name for name in names if name in _IGNORED_DIRECTORIES}


def copy_project(
    workspace: RunWorkspace, project_root: Path, source_root: Path
) -> Path:
    relative_project = project_root.resolve().relative_to(source_root.resolve())
    destination = workspace.generated / relative_project
    shutil.copytree(
        project_root,
        destination,
        dirs_exist_ok=True,
        ignore=_ignore_copy,
    )
    return destination


def _run_checked(command: list[str], *, cwd: Path) -> subprocess.CompletedProcess[str]:
    completed = subprocess.run(
        command,
        cwd=cwd,
        capture_output=True,
        text=True,
        check=False,
    )
    if completed.returncode != 0:
        detail = (completed.stderr or completed.stdout).strip()
        if len(detail) > 4000:
            detail = detail[-4000:]
        raise AlgoVizError(
            f"Command failed ({' '.join(command)}):\n{detail or 'no output'}"
        )
    return completed


def _ctest_commands(build_directory: Path) -> list[list[str]]:
    completed = _run_checked(
        ["ctest", "--test-dir", str(build_directory), "--show-only=json-v1"],
        cwd=build_directory,
    )
    try:
        metadata = json.loads(completed.stdout)
        return [test["command"] for test in metadata.get("tests", [])]
    except (KeyError, TypeError, json.JSONDecodeError) as error:
        raise AlgoVizError("CTest returned invalid test metadata.") from error


def _listed_gtests(output: str) -> set[str]:
    tests: set[str] = set()
    suite = ""
    for line in output.splitlines():
        content = line.split("#", 1)[0].rstrip()
        if not content:
            continue
        if not line[:1].isspace() and content.endswith("."):
            suite = content
        elif suite and line[:1].isspace():
            tests.add(f"{suite}{content.strip()}")
    return tests


def _find_test_command(build_directory: Path, test_filter: str) -> list[str]:
    available: set[str] = set()
    for command in _ctest_commands(build_directory):
        completed = subprocess.run(
            [*command, "--gtest_list_tests"],
            cwd=build_directory,
            capture_output=True,
            text=True,
            check=False,
        )
        listed = _listed_gtests(completed.stdout)
        available.update(listed)
        if test_filter in listed:
            return command

    detail = ", ".join(sorted(available)) or "none"
    raise AlgoVizError(
        f"GoogleTest scenario '{test_filter}' was not found. Available tests: {detail}."
    )


def _read_trace(path: Path) -> list[dict[str, object]]:
    events: list[dict[str, object]] = []
    if not path.exists():
        return events
    for line_number, line in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
        try:
            events.append(json.loads(line))
        except json.JSONDecodeError as error:
            raise AlgoVizError(
                f"Invalid trace event at {path}:{line_number}."
            ) from error
    return events


def run_gtest_trace(
    *,
    workspace: RunWorkspace,
    input_path: Path,
    source_root: Path,
    test_filter: str,
    timeout_seconds: float,
) -> RuntimeResult:
    project_root = find_cmake_project(input_path, source_root)
    copied_project = copy_project(workspace, project_root, source_root)
    copied_input = workspace.generated / input_path.resolve().relative_to(source_root.resolve())
    definitions = BootstrapFunctionDiscovery().discover(copied_input)
    write_trace_header(copied_input.parent / TRACE_HEADER_NAME)
    instrument_source(copied_input, definitions)

    _run_checked(
        [
            "cmake",
            "-S",
            str(copied_project),
            "-B",
            str(workspace.build),
            "-DCMAKE_BUILD_TYPE=Debug",
        ],
        cwd=workspace.root,
    )
    _run_checked(
        ["cmake", "--build", str(workspace.build), "--parallel"],
        cwd=workspace.root,
    )
    test_command = _find_test_command(workspace.build, test_filter)
    trace_path = workspace.trace / "events.jsonl"
    trace_path.touch()
    environment = os.environ.copy()
    environment["ALGOVIZ_TRACE_FILE"] = str(trace_path)
    command = [*test_command, f"--gtest_filter={test_filter}", "--gtest_color=no"]

    try:
        completed = subprocess.run(
            command,
            cwd=workspace.build,
            env=environment,
            capture_output=True,
            text=True,
            check=False,
            timeout=timeout_seconds,
        )
        status = "passed" if completed.returncode == 0 else "failed"
        exit_code = completed.returncode
        stdout = completed.stdout
        stderr = completed.stderr
    except subprocess.TimeoutExpired as error:
        status = "timed_out"
        exit_code = None
        stdout = error.stdout.decode() if isinstance(error.stdout, bytes) else (error.stdout or "")
        stderr = error.stderr.decode() if isinstance(error.stderr, bytes) else (error.stderr or "")

    return RuntimeResult(
        status=status,
        exit_code=exit_code,
        stdout=stdout,
        stderr=stderr,
        trace_path=trace_path,
        events=_read_trace(trace_path),
        test_command=command,
    )