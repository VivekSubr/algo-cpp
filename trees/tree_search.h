#pragma once
#include "binary_tree.h"

template <typename T>
bool dfs(const BinaryTree<T>& tree, const T& value, int index=0)
{
    if (index >= tree.getMaxIndex())   return false;
    if (*tree.getNode(index) == value) return true;

    // Search in left and right subtrees
    return dfs(tree, value, 2 * index + 1) || dfs(tree, value, 2 * index + 2);
}

template <typename T>
bool bfs(const BinaryTree<T>& tree, const T& value)
{
    std::deque<int> Q;
    Q.push_back(0);

    while(!Q.empty())
    {
        int node = Q.front(); Q.pop_front();
        if(node >= tree.getMaxIndex()) continue;

        if(*tree.getNode(node) == value) return true;

        Q.push_back(2 * node + 1); // left
        Q.push_back(2 * node + 2); // right
    }

    return false;
}


