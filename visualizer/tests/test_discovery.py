import tempfile
import unittest
from pathlib import Path

from algoviz.discovery import BootstrapFunctionDiscovery, select_definitions
from algoviz.errors import SymbolResolutionError


class BootstrapFunctionDiscoveryTests(unittest.TestCase):
    def test_discovers_free_functions_and_methods(self) -> None:
        source = """
        int add(int left, int right) {
            return left + right;
        }

        class Counter {
        public:
            Counter(int value) : value_(value) {}
            int increment(int amount) {
                auto callback = [amount](int value) { return value + amount; };
                return callback(value_);
            }
        private:
            int value_;
        };
        """
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "sample.h"
            path.write_text(source, encoding="utf-8")

            definitions = BootstrapFunctionDiscovery().discover(path)

        names = [definition.qualified_name for definition in definitions]
        self.assertEqual(names, ["add", "Counter::Counter", "Counter::increment"])

    def test_rejects_an_ambiguous_overload(self) -> None:
        source = """
        int transform(int value) { return value; }
        double transform(double value) { return value; }
        """
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "sample.h"
            path.write_text(source, encoding="utf-8")
            definitions = BootstrapFunctionDiscovery().discover(path)

        with self.assertRaises(SymbolResolutionError):
            select_definitions(definitions, "transform")


if __name__ == "__main__":
    unittest.main()
