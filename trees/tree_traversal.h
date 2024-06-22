#pragma once
#include "binary_tree.h"
#include <deque>

template <typename T>
void do_preOrderTraversal(const BinaryTree<T>& tree, int index, std::vector<std::shared_ptr<T>>& traversal)
{
    if(tree.getAt(index) != nullptr)
    {
        traversal.push_back(tree.getAt(index));
        
        int left = tree.getLeftChild(index).first;
        if(left > 0) do_preOrderTraversal(tree, left, traversal);

        int right = tree.getRightChild(index).first;
        if(right > 0) do_preOrderTraversal(tree, tree.getRightChild(index).first, traversal);
    }
}

template <typename T>
void do_postOrderTraversal(const BinaryTree<T>& tree, int index, std::vector<std::shared_ptr<T>>& traversal)
{
    if(tree.getAt(index) != nullptr)
    {
        int left = tree.getLeftChild(index).first;
        if(left > 0) do_postOrderTraversal(tree, left, traversal);

        int right = tree.getRightChild(index).first;
        if(right > 0) do_postOrderTraversal(tree, tree.getRightChild(index).first, traversal);

        traversal.push_back(tree.getAt(index));
    }
}

template <typename T>
void do_inOrderTraversal(const BinaryTree<T>& tree, int index, std::vector<std::shared_ptr<T>>& traversal)
{
    if(tree.getAt(index) != nullptr)
    {
        int left = tree.getLeftChild(index).first;
        if(left > 0) do_inOrderTraversal(tree, left, traversal);

        traversal.push_back(tree.getAt(index));

        int right = tree.getRightChild(index).first;
        if(right > 0) do_inOrderTraversal(tree, tree.getRightChild(index).first, traversal);
    }
}

/*
    Visit node.
    Go to the left subtree.
    Go to the right subtree.

    preOrder is thus top down, as we first visit before doing dfs
*/
template <typename T>
std::vector<std::shared_ptr<T>> preOrderTraversal(const BinaryTree<T>& tree)
{
    std::vector<std::shared_ptr<T>> ret;
    do_preOrderTraversal<T>(tree, 0, ret);
    
    return ret;
}

/*
    Go to the left subtree.
    Go to the right subtree.
    Visit the node.

    postOrder is thus bottom up, as node is visited at end of DFS
*/
template <typename T>
std::vector<std::shared_ptr<T>> postOrderTraversal(const BinaryTree<T>& tree)
{
    std::vector<std::shared_ptr<T>> ret;
    do_postOrderTraversal<T>(tree, 0, ret);
    
    return ret;
}

/*
    Go to the left subtree.
    Visit node.
    Go to the right subtree.
*/
template <typename T>
std::vector<std::shared_ptr<T>> inOrderTraversal(const BinaryTree<T>& tree)
{
    std::vector<std::shared_ptr<T>> ret;
    do_inOrderTraversal<T>(tree, 0, ret);
    
    return ret;
}

/*
    Visit nodes on each level left to right in order of levels
*/
template <typename T>
std::vector<std::shared_ptr<T>> levelOrderTraversal(const BinaryTree<T>& tree)
{
    std::vector<std::shared_ptr<T>> ret;
    std::deque<int> Q;
   
    Q.push_back(0);
    ret.push_back(tree.getRoot());
    while(!Q.empty())
    {
        int node = Q.front(); Q.pop_front();
        
        auto left = tree.getLeftChild(node);
        if(left.second)
        {
            Q.push_back(left.first);
            ret.push_back(left.second);
        }

        auto right = tree.getRightChild(node);
        if(right.second)
        {
            Q.push_back(right.first);
            ret.push_back(right.second);
        }
    }

    return ret;
}
