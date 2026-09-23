Some things to keep top of mind when doing problems using C++

# Iterators
Iterators are a wrapper for a pointer to a sequence of elements, usually std:: containers. C++ classifies iterators according to what operations they can perform.

| Iterator category | Properties and examples |
| --- | --- |
| Input | Single-pass, read-only, forward-only (`istream_iterator`) |
| Output | Single-pass, write-only (`ostream_iterator`, `back_inserter`) |
| Forward | Multi-pass, read/write, forward-only (`forward_list`) |
| Bidirectional | Adds `--` (`list`, `map`, `set`) |
| Random access | Adds `+`, `-`, `[]`, full arithmetic (`vector`, `deque`) |
| Contiguous (C++17/20) | Random access and guarantees elements are laid out contiguously in memory (`vector`, `array`, `string`) |

**Iterator invalidation**
Invalidation is what happens when the container mutates its internal representation in a way that the iterator's stored address/node no longer correctly identifies the position it used to.

Using a invalidated iterator is undefined behavior.

For continious containers (vector, string, deque) - all current iterators are invalidated by reallocation or erasing elements in the middle (push_back, insert_front don't)

For node based containers (linked_list, map) - almost never invalidated, unless the exact element iterator points to is erased.

For hash tables (unordered_map, unordered_set) - iterators are invalidated when elements are *re-hashed*, which can be on insertion or erasure, it isn't really deterministic.

# std::map
std::map spec gurantees the following properties

**Ordering**
Elements are always kept sorted by key according to Compare (default std::less<Key>). Iteration is in ascending key order.

Change the ordering by changing the comparator, eg: change to ascending by std::map<int, int, std::greater<int>>

**Uniqueness**
Gurantees unique keys, defined using Compare, Comp(a, b) != Comp(b, a)

**Complexity**
O(log n) for find, insert, erase, count, lower_bound/upper_bound. operator[]

Rembered, hashmaps are O(1), this is not a hashmap.

**Iterator/reference stability**
Insertion doesn't invalidate existing iterators/references; erasure only invalidates iterators to the erased element(s).
