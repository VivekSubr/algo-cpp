from __future__ import annotations

import argparse
import sys
from pathlib import Path
from typing import Sequence

from algoviz.discovery import BootstrapFunctionDiscovery, select_definitions
from algoviz.errors import AlgoVizError
from algoviz.report import open_report, write_report
from algoviz.runtime import run_gtest_trace
from algoviz.workspace import (
    RunWorkspace,
    SourceIntegrityGuard,
    find_source_root,
)


def _parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        prog="algoviz",
        description="Analyze C++ algorithms without modifying their source files.",
    )
    parser.add_argument("file", type=Path, help="C++ source or header file")
    parser.add_argument(
        "function",
        nargs="?",
        help="Function name; omit to select all definitions in the file",
    )
    parser.add_argument(
        "--list",
        action="store_true",
        help="List selected definitions without executing a scenario",
    )
    parser.add_argument(
        "--test",
        metavar="GTEST_FILTER",
        help="GoogleTest scenario used to drive the selected function",
    )
    parser.add_argument(
        "--timeout",
        type=float,
        default=5.0,
        metavar="SECONDS",
        help="Maximum scenario runtime (default: 5 seconds)",
    )
    parser.add_argument(
        "--no-open",
        action="store_true",
        help="Write the HTML report without opening it",
    )
    parser.add_argument(
        "--data-dir",
        type=Path,
        help=argparse.SUPPRESS,
    )
    return parser


def _print_definitions(definitions: list[object]) -> None:
    print(f"{'FUNCTION':<36} {'LINE':>6}  SIGNATURE")
    for definition in definitions:
        print(
            f"{definition.qualified_name:<36} "
            f"{definition.line:>6}  {definition.signature}"
        )


def run(args: argparse.Namespace) -> int:
    input_path = args.file.resolve()
    if not input_path.is_file():
        raise AlgoVizError(f"Input file does not exist: '{input_path}'.")
    if input_path.suffix.lower() not in {".c", ".cc", ".cpp", ".cxx", ".h", ".hh", ".hpp"}:
        raise AlgoVizError(f"Input is not a recognized C/C++ file: '{input_path}'.")
    if not args.list and not args.test:
        raise AlgoVizError(
            "Runtime tracing requires --test <gtest-filter>; use --list for discovery only."
        )
    if args.timeout <= 0:
        raise AlgoVizError("--timeout must be greater than zero.")

    source_root = find_source_root(input_path)
    workspace = RunWorkspace.create(source_root, args.data_dir)
    runtime_result = None
    report_path = None

    with SourceIntegrityGuard(input_path) as integrity:
        snapshot = workspace.snapshot(input_path, source_root)
        discovered = BootstrapFunctionDiscovery().discover(snapshot)
        selected = select_definitions(discovered, args.function)
        manifest: dict[str, object] = {
            "schemaVersion": 1,
            "input": str(input_path),
            "inputSha256": integrity.initial_hash,
            "snapshot": str(snapshot),
            "selectedDefinitions": [
                definition.as_dict() for definition in selected
            ],
            "sourceRoot": str(source_root),
            "status": "discovered",
        }

        if not args.list:
            runtime_result = run_gtest_trace(
                workspace=workspace,
                input_path=input_path,
                source_root=source_root,
                test_filter=args.test,
                timeout_seconds=args.timeout,
            )
            report_path = write_report(
                workspace=workspace,
                source_path=input_path,
                selected=selected,
                test_filter=args.test,
                result=runtime_result,
            )
            manifest.update(
                {
                    "status": runtime_result.status,
                    "test": args.test,
                    "testCommand": runtime_result.test_command,
                    "testExitCode": runtime_result.exit_code,
                    "trace": str(runtime_result.trace_path),
                    "traceEvents": len(runtime_result.events),
                    "report": str(report_path),
                }
            )
        workspace.write_manifest(manifest)

    _print_definitions(selected)
    print(f"\nRun workspace: {workspace.root}")
    if runtime_result is not None and report_path is not None:
        print(
            f"Scenario: {args.test} ({runtime_result.status}, "
            f"{len(runtime_result.events)} trace events)"
        )
        print(f"Report: {report_path}")
        if not args.no_open and not open_report(report_path):
            print("The report could not be opened automatically; open the path above.")
    return 0


def main(argv: Sequence[str] | None = None) -> int:
    args = _parser().parse_args(argv)
    try:
        return run(args)
    except AlgoVizError as error:
        print(f"algoviz: error: {error}", file=sys.stderr)
        return 2
