# Union Find

**What it does**
For a bunch of items and want to answer two questions quickly:
    Union(a, b): put a and b in the same group.
    Find(a):     which group is a in? (Are a and b in the same group?)

UnionFind maintains two arrays - parents and rank.
    * parrent[i] = j, means parrent of ith node is jth node.
    * rank[i]    = size of connected component.

<iframe src="./union-find-animation.html" width="100%" height="600" title="Union-Find animation"></iframe>


## Path Compression
Current implementation in union_find.h does not have path compression... find basically walks the entire path.

```
    int find(int node)
    {
        int ret = node; //If completely disjoint, node is own parent
        while(ret != m_parentArr[ret])
        { //go up till we find the root
            ret = m_parentArr[ret]; 
        }

        return ret;
    }
```

Path compression is a form of memoization, we set m_parentArr[node] = ret so that next time it can directly return.