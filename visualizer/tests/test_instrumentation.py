import tempfile
import unittest
from pathlib import Path

from algoviz.discovery import BootstrapFunctionDiscovery
from algoviz.instrumentation import instrument_source, write_trace_header


class InstrumentationTests(unittest.TestCase):
    def test_instruments_member_and_global_state(self) -> None:
        source = """int global_count = 1;
namespace config {
int limit = 2;

class Heap {
    using Base::helper;
    using Base::m_tree;
    int m_type;
public:
    void build(int amount)
    {
        m_tree.clear();
        helper();
        int m_type = amount;
        global_count += m_type;
    }

    static void reset()
    {
        global_count = 0;
    }
};
}
"""
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "sample.cc"
            path.write_text(source, encoding="utf-8")
            definitions = BootstrapFunctionDiscovery().discover(path)

            instrument_source(path, definitions)

            instrumented = path.read_text(encoding="utf-8")

        build_start = instrumented.index('record("config::Heap::build"')
        build_end = instrumented.index('record("config::Heap::reset"')
        build = instrumented[build_start:build_end]
        reset = instrumented[build_end:]
        self.assertIn('field("::global_count", ::global_count)', build)
        self.assertIn('field("config::limit", ::config::limit)', build)
        self.assertIn('field("this->m_tree", this->m_tree)', build)
        self.assertIn('field("this->m_type", this->m_type)', build)
        self.assertIn('field("m_type", m_type)', build)
        self.assertNotIn('field("helper",', build)
        self.assertNotIn("this->", reset)

    def test_preserves_if_else_adjacency(self) -> None:
        source = """int choose(bool first, bool second)
{
    if(first)
    {
        return 1;
    }
    else if(second) return 2;
    return 0;
}
"""
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "sample.cc"
            path.write_text(source, encoding="utf-8")
            definitions = BootstrapFunctionDiscovery().discover(path)

            instrument_source(path, definitions)

            instrumented = path.read_text(encoding="utf-8")

        self.assertIn("    }\n    else if(second) return 2;", instrumented)

    def test_shields_trace_header_from_access_specifier_macros(self) -> None:
        source = """int value()
{
    return 1;
}
"""
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "sample.cc"
            path.write_text(source, encoding="utf-8")
            definitions = BootstrapFunctionDiscovery().discover(path)

            instrument_source(path, definitions)

            instrumented = path.read_text(encoding="utf-8")

        self.assertTrue(
            instrumented.startswith(
                '#pragma push_macro("private")\n'
                "#undef private\n"
                '#pragma push_macro("protected")\n'
                "#undef protected\n"
                '#include "algoviz_trace.h"\n'
                '#pragma pop_macro("protected")\n'
                '#pragma pop_macro("private")\n'
            )
        )

    def test_instruments_one_line_function_inside_its_body(self) -> None:
        source = """class Heap {
public:
    const int& getArray() const { return m_tree; }
private:
    int m_tree;
};
"""
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "sample.cc"
            path.write_text(source, encoding="utf-8")
            definitions = BootstrapFunctionDiscovery().discover(path)

            instrument_source(path, definitions)

            instrumented = path.read_text(encoding="utf-8")

        function_start = instrumented.index("getArray")
        opening_brace = instrumented.index("{", function_start)
        trace = instrumented.index('::algoviz_trace::record("Heap::getArray"')
        return_statement = instrumented.index("return m_tree", function_start)
        closing_brace = instrumented.index("}", return_statement)
        self.assertLess(opening_brace, trace)
        self.assertLess(trace, return_statement)
        self.assertLess(return_statement, closing_brace)

    def test_instruments_parameters_locals_and_loop_variables(self) -> None:
        source = """int total(int limit)
{
    int result = 0;
    for(int index = 0; index < limit; ++index)
    {
        int amount = index;
        result += amount;
    }
    return result;
}
"""
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "sample.cc"
            path.write_text(source, encoding="utf-8")
            definitions = BootstrapFunctionDiscovery().discover(path)

            instrument_source(path, definitions)

            instrumented = path.read_text(encoding="utf-8")

        self.assertIn('field("limit", limit)', instrumented)
        self.assertIn('field("result", result)', instrumented)
        self.assertIn('field("index", index)', instrumented)
        self.assertIn('field("amount", amount)', instrumented)
        self.assertNotIn('field("t", t)', instrumented)
        self.assertNotIn('field("e", e)', instrumented)

    def test_writes_linked_list_trace_support(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "algoviz_trace.h"
            write_trace_header(path)
            header = path.read_text(encoding="utf-8")

        self.assertIn('\\"linked-list\\"', header)
        self.assertIn("visited", header)

    def test_writes_container_and_pointer_trace_support(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "algoviz_trace.h"
            write_trace_header(path)
            header = path.read_text(encoding="utf-8")

        self.assertIn("struct is_iterable", header)
        self.assertIn("std::is_pointer_v", header)
        self.assertIn("range_json(value)", header)


if __name__ == "__main__":
    unittest.main()