# Trees
Trees are usually O(logn), since algos at every node decide on one of 2. 

std::map, std::set give red-black trees in C++.

## Tree Traversal
**pre-order**

**post-order**

**in-order**


All of these are O(logn)

## Tree Search 


# Heap 
A binary heap is a **complete** binary tree (every level full except possibly the last, which fills left-to-right) satisfying the **heap property**

* In a max-heap, parent >= child
* In a min-heap, parent <= child

Complete binary trees are typically represented using arrays, since because of their completeness they don't have null elements in between. 

Flattened into a plain array in level order, and the parent/child relationships become arithmetic on the index i:
    * left child:  2i + 1
    * right child: 2i + 2
    * parent:     (i - 1) / 2 

Now let's see the heapify code:

```
    void build_heap(const std::vector<node_t>& arr)
    {
        m_tree.clear();
        m_tree = arr;

        //heapify only the leaf nodes as heapify traverses till the top anyway.
        for(int i=getLastNonLeafIndex(); i>=0; i--) //i--, we are going up!
        {
            heapify(i);
        }
    }

    void heapify(size_t index)
    {
        size_t left  = getLeftChildIndex(index);
        size_t right = getRightChildIndex(index);

        if(left < m_tree.size() && compare(getAt(index), getAt(left)))
        {
            std::swap(m_tree[index], m_tree[left]);
            heapify(left);
        }

        if(right < m_tree.size() && compare(getAt(index), getAt(right)))
        {
            std::swap(m_tree[index], m_tree[right]);
            heapify(right);
        }
    }
```

First for we don't need to start at leaf node... we simply array the remaining nodes so that leaves are in right place, means approx half of the array is gone right up.

Hence, for remaining n/2 elements, swap is done n/2 times... O(n).

## Heap in C++
There's no std::heap, instead we have std::make_heap that works on any container with random access iterator (std::vector, std::array, std::deque).

```
std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};

std::make_heap(v.begin(), v.end());        // O(n) — arrange into a valid max-heap
v.push_back(8);
std::push_heap(v.begin(), v.end());        // O(log n) — sift the new last element up

std::pop_heap(v.begin(), v.end());         // O(log n) — moves max to v.back(), e-heapifies the rest

v.pop_back();                              // actually remove it

std::sort_heap(v.begin(), v.end());        // O(n log n) — repeated pop_heap; result is sorted, no longer a heap... basically heap sort
```

Other than this, we have std::priority_queue, which is again a heap
```
std::priority_queue<int> maxpq;                                      // max-heap
std::priority_queue<int, std::vector<int>, std::greater<int>> minpq; // min-heap
```

Used in Djikstra's algo.

