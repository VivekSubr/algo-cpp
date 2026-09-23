import tempfile
import unittest
from pathlib import Path

from algoviz.models import FunctionDefinition
from algoviz.report import write_report
from algoviz.runtime import RuntimeResult
from algoviz.workspace import RunWorkspace


class ReportTests(unittest.TestCase):
    def test_writes_self_contained_trace_player(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            base = Path(directory)
            source_root = base / "repository"
            source_root.mkdir()
            source = source_root / "sample.cc"
            source.write_text("int run() { return 1; }\n", encoding="utf-8")
            workspace = RunWorkspace.create(source_root, base / "runs")
            result = RuntimeResult(
                status="passed",
                exit_code=0,
                stdout="ok",
                stderr="",
                trace_path=workspace.trace / "events.jsonl",
                events=[
                    {
                        "step": 0,
                        "function": "run",
                        "line": 1,
                        "variables": {
                            "answer": 1,
                            "head": {
                                "kind": "linked-list",
                                "head": "node-1",
                                "nodes": [
                                    {"id": "node-1", "value": 1, "next": "node-2"},
                                    {"id": "node-2", "value": 2, "next": None},
                                ],
                                "cycle": False,
                            },
                        },
                    }
                ],
                test_command=["sample_test"],
            )

            report = write_report(
                workspace=workspace,
                source_path=source,
                selected=[FunctionDefinition("run", "run", "run()", 1)],
                test_filter="Sample.Run",
                result=result,
            )
            html = report.read_text(encoding="utf-8")

        self.assertIn("AlgoViz trace", html)
        self.assertIn("Sample.Run", html)
        self.assertIn('"answer":1', html)
        self.assertIn('id="scrubber"', html)
        self.assertIn('aria-label="Next step">&#9654;</button>', html)
        self.assertNotIn("scrollIntoView", html)
        self.assertIn("function renderObjectGraph", html)
        self.assertIn("All linked-list nodes and connections", html)
        self.assertIn("last observed", html)
        self.assertIn(
            ".inspect-panel {\n"
            "  position: sticky;\n"
            "  top: 16px;\n"
            "  align-self: start;\n"
            "  max-height: calc(100vh - 32px);\n"
            "}",
            html,
        )
        self.assertIn(
            ".inspect-panel { position: static; max-height: none; }", html
        )


if __name__ == "__main__":
    unittest.main()