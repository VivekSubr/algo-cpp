# AlgoViz implementation plan

## Goal

Build a command-line tool that visualizes the execution of C++ algorithms and
estimates their complexity.

```text
algoviz <file-name> [function-name]
```

If `function-name` is omitted, every function defined in the file is selected.
Functions introduced only through an `#include` are not selected.

The first supported repository is `algo-cpp`, including its free functions,
classes, templates, STL containers, recursion, smart-pointer structures,
graphs, trees, linked lists, matrices, and GoogleTest scenarios.

## Non-negotiable safety invariant

AlgoViz must never modify an input file or create build artifacts in the input
repository.

Every invocation must:

1. Resolve and hash the requested input.
2. Reject a run-data directory located inside the source repository.
3. Create an external, per-run workspace.
4. Copy the required sources into that workspace.
5. Analyze, generate, compile, and execute only in the workspace.
6. Hash the original input again before completing.
7. Fail the run if the original hash changed.

For hosted execution, the source tree will additionally be mounted read-only.
Generated harnesses, compiler output, traces, and reports belong under the
external run directory.

## User experience

```powershell
algoviz Graph\graph_traversal.h bfs
algoviz Graph\graph.h GraphList::addEdge
algoviz dynamic_programming\nqueen.h
algoviz Graph\graph_traversal.h --list
```

Planned options:

```text
--list                   List definitions and matching tests
--test <gtest-filter>    Select an existing GoogleTest scenario
--input <json-file>      Supply arguments to a generated harness
--static-only            Analyze without executing
--output <directory>     Export a trace and report
--no-open                Do not launch the visualization UI
```

An unqualified function name is accepted only when it identifies one
definition. Ambiguous overloads or methods produce an error listing valid
qualified signatures.

## Architecture

### CLI and orchestration

- Python 3.12
- Standard-library CLI during the bootstrap phase
- FastAPI and WebSocket transport when the UI is introduced

The CLI owns path validation, run lifecycle, test selection, process limits,
trace streaming, and integrity verification.

### C++ discovery and static analysis

- Clang frontend and LibTooling
- `compile_commands.json` generated in the external workspace
- Clang AST used for definitions, source ranges, types, call relationships,
  loops, branches, recursion, and complexity evidence

The bootstrap implementation contains a dependency-free lexical discovery
provider. It exists only to establish the CLI and safety boundary before Clang
is provisioned. Its interface will be replaced by the Clang provider; it is not
the final C++ parser.

### Runtime tracing

- Compile with Clang using `-O0 -g`
- Discover tests using GoogleTest `--gtest_list_tests`
- Execute scenarios using `--gtest_filter`
- Use the LLDB Python API for source stepping, stack frames, arguments, locals,
  return values, and object identity
- Add a Clang plugin or LLVM pass later for efficient operation-level events

The selected function is a trace root. Its transitive calls are retained while
GoogleTest internals and unrelated setup are hidden by default. In file mode,
all definitions in the file are trace roots and relevant tests run only once.

### Trace model

Versioned JSON Lines events:

```json
{"version":1,"step":12,"kind":"line","file":"graph_traversal.h","line":21}
{"version":1,"step":13,"kind":"variable","frame":2,"name":"node","value":2}
{"version":1,"step":14,"kind":"call","function":"Graph::getNeighbors"}
{"version":1,"step":15,"kind":"return","function":"bfs","value":[2,0,3,1]}
```

Large values will use object IDs and deltas rather than repeating complete
snapshots.

### Visualization

- React, TypeScript, and Vite
- Monaco Editor for source display
- D3.js for arrays, matrices, timelines, and custom layouts
- Cytoscape.js for graphs, trees, linked lists, and pointer relationships
- Zustand for deterministic trace playback

All values receive a generic inspector. Recognized shapes additionally receive
specialized views:

| Runtime shape | Specialized view |
| --- | --- |
| Array, `vector`, `deque` | Indexed cells or bar chart |
| Nested sequence | Matrix or grid |
| Stack, queue, priority queue | Container view |
| Map or set | Entries or buckets |
| `shared_ptr` object graph | Nodes and pointer edges |
| `left` and `right` members | Tree |
| `next` member | Linked list |
| Graph edges or adjacency data | Graph |
| Boolean matrix | DP or backtracking board |

Unknown classes remain visualizable through the generic object graph.

### Complexity estimation

Static evidence:

- Loop nesting and bounds
- Recursive call structure
- Branch-dependent best and worst cases
- Standard-library operation cost models
- Explicit uncertainty for virtual dispatch or unresolved bounds

Observed evidence:

- Executed statements
- Comparisons and assignments
- Function calls and recursion depth
- Allocations and container operations

Asymptotic fitting requires scalable scenarios. Operation counts are fitted
against `1`, `log n`, `n`, `n log n`, `n^2`, `n^3`, and `2^n`. Fixed tests can
produce a trace and static estimate, but cannot establish empirical growth.

## Delivery milestones

### M0: CLI and immutable workspace

- Package and `algoviz` entry point
- File and optional function arguments
- External per-run workspace
- Input hashing before and after analysis
- Source snapshot and run manifest
- Bootstrap function discovery and deterministic symbol resolution
- Unit tests proving the original input remains unchanged

### M1: Clang discovery

- Toolchain detection with actionable diagnostics
- Compilation database ingestion
- Clang AST definition provider
- Qualified methods, overloads, templates, and source ranges
- Static call graph and control-flow metadata

### M2: Repository build and scenarios

- Out-of-tree CMake/Ninja builds
- Map headers to their test translation units
- GoogleTest discovery and filtering
- Generated harnesses for supported JSON argument types
- Explicit reporting for unexecutable definitions

### M3: Runtime trace

- LLDB driver and repository-source filtering
- Line, call, return, local-variable, and stack events
- STL and smart-pointer value serialization
- Time, memory, recursion-depth, and event-count limits

### M4: Web visualization

- FastAPI/WebSocket trace service
- React playback shell and Monaco source synchronization
- Generic value inspector
- Array, matrix, linked-list, tree, and graph renderers

### M5: Complexity

- AST-based symbolic estimate with evidence
- Runtime operation counters
- Scalable scenario format and empirical curve fitting
- Confidence and uncertainty reporting

### M6: Hardening

- Docker read-only source mounts
- gVisor or equivalent hosted isolation
- CPU, memory, process, filesystem, and output limits
- Trace schema compatibility and deterministic replay tests

## MVP acceptance criteria

- `algoviz <file> [function]` works from the repository.
- Omitting the function selects all definitions physically present in the file.
- Ambiguous or missing symbols fail clearly without guessing.
- Existing GoogleTest scenarios can drive functions requiring arguments.
- Every executed path has source, stack, and variable visualization.
- Recognized repository structures receive specialized visualizations.
- Unrecognized structures still receive a generic object view.
- Static complexity includes evidence and uncertainty.
- Empirical complexity is shown only when scalable inputs exist.
- Input repositories remain byte-for-byte unchanged.

## Explicit boundaries

- Commented-out or uncompilable code cannot produce a runtime trace.
- Code not reached by a selected test or harness receives static analysis only.
- Exact asymptotic complexity cannot be guaranteed for arbitrary C++.
- No parser fallback may silently claim Clang-level accuracy.
