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

## Recursive vs Non-Recursive
Tree algos often look intuitively recursive, because of the natural hierachical nature of trees, but can be done non-recursively using stacks or queues, which push and pop up and down the tree.

eg, inverting a tree, recursive
```
void invert(TreeNode* root)
{
    if(root == nullptr) return;
        
    TreeNode* left  = root->left;
    TreeNode* right = root->right;

    root->left = right;
    root->right = left;

    invert(left);
    invert(right);
}

TreeNode* invertTree(TreeNode* root) 
{
    invert(root);
    return root;
}
```

Non-recursive DFS, basically pushing in children and poping root.
```
TreeNode* invertTree(TreeNode* root)
{
    if (root == nullptr) return root;

    std::stack<TreeNode*> stk;
    stk.push(root);

    while (!stk.empty())
    {
        TreeNode* node = stk.top();
        stk.pop();

        std::swap(node->left, node->right);

        if (node->left)  stk.push(node->left);
        if (node->right) stk.push(node->right);
    }
    return root;
}
```

BFS version, level by level
```
TreeNode* invertTree(TreeNode* root)
{
    if (root == nullptr) return root;

    std::queue<TreeNode*> q;
    q.push(root);

    while (!q.empty())
    {
        TreeNode* node = q.front();
        q.pop();

        std::swap(node->left, node->right);

        if (node->left)  q.push(node->left);
        if (node->right) q.push(node->right);
    }
    return root;
}
```