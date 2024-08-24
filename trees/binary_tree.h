#pragma once
#include <vector>
#include <memory>
#include <cmath>
#include <iostream>

template <typename T>
class BinaryTree
{
/*
    Array representation of binary tree
    The left child of node i can be found at position 2i + 1
    The right child of node i can be found at position 2i + 2
    The parent of node i can be found at position (i – 1) / 2
*/

/*
    Properties of Binary Trees
    *  The maximum number of nodes at level ‘l’ of a binary tree is 2^l
    *  The Maximum number of nodes in a binary tree of height ‘h’ is 2^h – 1 
    *  In a Binary Tree with N nodes, the minimum possible height or the minimum number of levels is Log2(N+1)
    *  A Binary Tree with L leaves has at least | Log2L |+ 1 levels
    *  In a Binary tree where every node has 0 or 2 children, the number of leaf nodes is always one more than nodes with two children
*/

public:
    BinaryTree(const std::vector<std::shared_ptr<T>>& arr): m_tree(arr)
    {
    }

    std::shared_ptr<T> getRoot() const
    {
        if(m_tree.size() > 0) return m_tree[0];
        return nullptr;
    }

    std::shared_ptr<T> getAt(int index) const
    {
        if(index < 0 || static_cast<std::size_t>(index) <= m_tree.size())
        {
            return m_tree[index];
        }

        return nullptr;
    }

    std::pair<int, std::shared_ptr<T>> getLeftChild(int index) const
    {
        std::size_t child_index = getLeftChildIndex(index);
        if(child_index >= m_tree.size()) 
        {
            return {-1, nullptr};
        }

        return {child_index, m_tree[child_index]};
    }

    std::pair<int, std::shared_ptr<T>> getRightChild(int index) const
    {
        std::size_t child_index = getRightChildIndex(index);
        if(child_index >= m_tree.size()) 
        {
            return {-1, nullptr};
        }

        return {child_index, m_tree[child_index]};
    }

    std::pair<int, std::shared_ptr<T>> getParent(int index) const
    {
        int parent_index = std::floor((index - 1)/2);
        if(parent_index < 0) 
        {
            return {-1, nullptr};
        }

        return {parent_index, m_tree[parent_index]};
    }

    std::size_t getMaxIndex() const { return m_tree.size() - 1; }

    //Basically, second half of array will all be leaf nodes.
    std::size_t getLastNonLeafIndex() const { return std::floor(m_tree.size()/2); }

    void printTree() 
    {
        std::size_t currentLevel = 0;
        std::size_t maxPerLevel = power2(currentLevel);
        for(std::size_t i=0; i<m_tree.size(); i++)
        {
            if(i == maxPerLevel - 1)
            {
                std::cout<<"\n";
                ++currentLevel;
                maxPerLevel = power2(currentLevel);
            }

            std::cout<<" ";
            if(m_tree[i]) std::cout<<*m_tree[i];
            else          std::cout<<"null";
        }

        std::cout<<"\n";
    }

    int getNumberLevels()
    {
        return 0;
    }

protected:
    std::vector<std::shared_ptr<T>> m_tree; 

    std::size_t power2(int power)
    {
        return (1 << power );
    }

    int getLeftChildIndex(int index) const
    {
        return 2 * index + 1;
    }

    int getRightChildIndex(int index) const
    {
        return 2 * index + 2;
    }

    int getParentIndex(int index) const 
    {
        return std::floor(index/2);
    }
};
