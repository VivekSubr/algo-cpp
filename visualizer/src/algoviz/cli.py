from __future__ import annotations

import argparse
import sys
from pathlib import Path
from typing import Sequence

from algoviz.discovery import BootstrapFunctionDiscovery, select_definitions
from algoviz.errors import AlgoVizError
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
        help="List selected definitions (currently the implemented operation)",
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

    source_root = find_source_root(input_path)
    workspace = RunWorkspace.create(source_root, args.data_dir)

    with SourceIntegrityGuard(input_path) as integrity:
        snapshot = workspace.snapshot(input_path, source_root)
        discovered = BootstrapFunctionDiscovery().discover(snapshot)
        selected = select_definitions(discovered, args.function)
        workspace.write_manifest(
            {
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
        )

    _print_definitions(selected)
    print(f"\nRun workspace: {workspace.root}")
    if not args.list:
        print("Runtime tracing is planned but not implemented in this milestone.")
    return 0


def main(argv: Sequence[str] | None = None) -> int:
    args = _parser().parse_args(argv)
    try:
        return run(args)
    except AlgoVizError as error:
        print(f"algoviz: error: {error}", file=sys.stderr)
        return 2
