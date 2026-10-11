# Heap in C++
There's no std::heap, instead we have std::make_heap that works on any container with random access iterator (std::vector, std::array, std::deque).

```
std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};

std::make_heap(v.begin(), v.end());        // O(n) — arrange into a valid max-heap
v.push_back(8);
std::push_heap(v.begin(), v.end());        // O(log n) — sift the new last element up

std::pop_heap(v.begin(), v.end());         // O(log n) — moves max to v.back(), de-heapifies the rest

v.pop_back();                              // actually remove it

std::sort_heap(v.begin(), v.end());        // O(n log n) — repeated pop_heap; result is sorted, no longer a heap... basically heap sort
```

Other than this, we have std::priority_queue, which is again a heap
```
std::priority_queue<int> maxpq;                                      // max-heap
std::priority_queue<int, std::vector<int>, std::greater<int>> minpq; // min-heap
```

Used in Djikstra's algo.


# std::queue

# std::priority_queue
As noted, std::priority_queue is basically a heap, 

```
std::priority_queue<int> maxpq;                                      // max-heap
std::priority_queue<int, std::vector<int>, std::greater<int>> minpq; // min-heap
```

It has push(), pop() and top().

It doesn't have erase() or decrease_key/increase_key()

