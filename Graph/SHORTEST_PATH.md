# Shortest Path in Graph

The go-to algo for finding shortest path from A to B in a graph is **Djikstra's**, but it doesn't handle negative edges... if negative edges, the algo is **Bellman-Ford**.

Special case is DAG, Directed Acyclical Graphs - the way to go is topological sort,
1. Topologically sort the DAG 
2. Set dist[source] = 0 and every other dist = ∞.
3. Go through the vertices in topological order and relax each edge leaving each vertex. 

## Meaning of 'Relax' in Graph algorithms
Relaxing an edge (u, v) with weight w means checking whether going through u gives a shorter route to v than the best one found so far, and updating if it does.

Example - say d[A] = 2, d[B] = 10, and there is an edge A→B with weight 3. Relaxing it gives 2+3 = 5 < 10, so d[B] becomes 5. If the edge weighed 9, then 2+9 = 11 > 10 and nothing changes.

## Complexity of Djikstra's
We have a hashmap and a heap... std::unordered_map and std::priority_queue in C++.
Heap is min heap on distance.

Basically,
    * Initialize hashmap with all nodes distance as INF.
    * Push in node in heap.

    * Pop top of heap
    * Push neighbors into heap
    * Update hashmap with least distance from neighbors

Again, now we have a while loop acting on the heap of nodes, and an inner for loop acting on the edges... so creating O is some function of N and E.

Worst case is visting each edge once, so the for loop is O(E)... pushing into heap is logN operation, and the pop is O(1) and push is done worse case E times, hence O(ElogN) complexity.