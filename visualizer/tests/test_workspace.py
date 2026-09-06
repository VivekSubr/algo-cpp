import json
import tempfile
import unittest
from contextlib import redirect_stderr, redirect_stdout
from io import StringIO
from pathlib import Path

from algoviz.cli import main


class WorkspaceTests(unittest.TestCase):
    def test_cli_snapshots_without_modifying_the_input(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            base = Path(directory)
            repository = base / "repository"
            repository.mkdir()
            (repository / ".git").mkdir()
            source = repository / "algorithm.h"
            original = b"int double_value(int value) { return value * 2; }\n"
            source.write_bytes(original)
            data_root = base / "runs"

            stdout = StringIO()
            with redirect_stdout(stdout):
                result = main(
                    [
                        str(source),
                        "double_value",
                        "--list",
                        "--data-dir",
                        str(data_root),
                    ]
                )

            self.assertEqual(result, 0)
            self.assertEqual(source.read_bytes(), original)
            run_directories = list(data_root.iterdir())
            self.assertEqual(len(run_directories), 1)
            manifest = json.loads(
                (run_directories[0] / "manifest.json").read_text(encoding="utf-8")
            )
            self.assertEqual(
                manifest["selectedDefinitions"][0]["qualifiedName"],
                "double_value",
            )
            self.assertEqual(
                (run_directories[0] / "source" / "algorithm.h").read_bytes(),
                original,
            )

    def test_rejects_a_run_directory_inside_the_repository(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            repository = Path(directory) / "repository"
            repository.mkdir()
            (repository / ".git").mkdir()
            source = repository / "algorithm.h"
            source.write_text("void run() {}\n", encoding="utf-8")

            stderr = StringIO()
            with redirect_stderr(stderr):
                result = main(
                    [
                        str(source),
                        "--data-dir",
                        str(repository / ".algoviz"),
                    ]
                )

            self.assertEqual(result, 2)
            self.assertIn("outside the input repository", stderr.getvalue())


if __name__ == "__main__":
    unittest.main()
