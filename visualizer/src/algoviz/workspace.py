from __future__ import annotations

import hashlib
import json
import os
import shutil
import uuid
from dataclasses import dataclass
from pathlib import Path

from algoviz.errors import IntegrityError, WorkspaceError


def hash_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as input_file:
        for chunk in iter(lambda: input_file.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def find_source_root(path: Path) -> Path:
    for candidate in (path.parent, *path.parents):
        if (candidate / ".git").exists():
            return candidate
    return path.parent


def _is_within(path: Path, parent: Path) -> bool:
    try:
        path.relative_to(parent)
        return True
    except ValueError:
        return False


def default_data_root() -> Path:
    local_app_data = os.environ.get("LOCALAPPDATA")
    if local_app_data:
        return Path(local_app_data) / "algoviz" / "runs"
    return Path.home() / ".algoviz" / "runs"


@dataclass(frozen=True, slots=True)
class RunWorkspace:
    root: Path
    source: Path
    generated: Path
    build: Path
    trace: Path
    report: Path

    @classmethod
    def create(cls, source_root: Path, data_root: Path | None = None) -> RunWorkspace:
        resolved_source_root = source_root.resolve()
        resolved_data_root = (data_root or default_data_root()).resolve()
        if _is_within(resolved_data_root, resolved_source_root):
            raise WorkspaceError(
                "Run data directory must be outside the input repository."
            )

        root = resolved_data_root / uuid.uuid4().hex
        paths = {
            name: root / name
            for name in ("source", "generated", "build", "trace", "report")
        }
        for path in paths.values():
            path.mkdir(parents=True, exist_ok=False)
        return cls(root=root, **paths)

    def snapshot(self, input_path: Path, source_root: Path) -> Path:
        try:
            relative_path = input_path.resolve().relative_to(source_root.resolve())
        except ValueError as error:
            raise WorkspaceError(
                f"Input file is outside source root '{source_root}'."
            ) from error

        destination = self.source / relative_path
        destination.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(input_path, destination)
        return destination

    def write_manifest(self, manifest: dict[str, object]) -> Path:
        path = self.root / "manifest.json"
        path.write_text(
            json.dumps(manifest, indent=2, sort_keys=True) + "\n",
            encoding="utf-8",
        )
        return path


class SourceIntegrityGuard:
    def __init__(self, path: Path) -> None:
        self.path = path.resolve()
        self.initial_hash = ""

    def __enter__(self) -> SourceIntegrityGuard:
        self.initial_hash = hash_file(self.path)
        return self

    def __exit__(self, exc_type: object, exc: object, traceback: object) -> bool:
        final_hash = hash_file(self.path)
        if final_hash != self.initial_hash:
            raise IntegrityError(
                f"Input file changed during analysis: '{self.path}'."
            )
        return False
