# Graph Complexity
Graphs are composed of two things - nodes and edges, and graph algorhitms typically act on one of the two, and hence complexity is expressed as O(node or edge)

# Breadth First Search
In a Queue Q:
    1. Insert starting node
    2. Pop Node
    3. Check if target node:
        if is:  return
        if not: insert all it's neighbors
    4. goto step 2

Now we have two things - 
    * Visit nodes
    * Check neighbors
Basically,
```
while !Q.empty():
    1. Pop front and check 
    2. for each neighbor:
        if not visited: add to set
```

So two distict things, the loop runs for Edges, the search itself is done on nodes.

Hence complexity is best expressed as O(N + E).

# Depth First Search
For each neighbor in node:
    visit node
    recursive for neighors in node which are not visited.

So, loop runs for each edge for each node, so at first glance, complexity looks to be O(E*N)... but we are looping for only un-visited nodes, not again and again for each node.

Take a graph with 4 nodes and 4 edges: A-B, A-C, B-C, C-D. V × E would predict 16 edge checks. In reality the neighbor lists are:

A: B, C → 2
B: A, C → 2
C: A, B, D → 3
D: C → 1

The total is 8, which is 2E. That’s the edge cost. The node cost, O(N), is added on top, giving O(N + E).


