#pragma once
#include "binary_tree.h"
#include <algorithm>

enum class heapType 
{
    Max, // root is largest element
    Min 
};


//https://en.wikipedia.org/wiki/Binary_heap

template <typename T>
class Heap : public BinaryTree<T>
{
    using BinaryTree<T>::m_tree;

public:
    Heap(const std::vector<std::shared_ptr<T>>& arr, heapType type, bool isHeap = false): m_type(type), BinaryTree<T>(arr)
    {
        if(!isHeap) build_heap();
    }

private:
    heapType m_type;

    /*
       Creating a heap from a array is different from 'heapify'.
       Heapify is a operation on a *pre-existing* heap after push/pop to maintain heap properties.

       Now, naive way of building a heap is starting with an empty array and inserting all n nodes. 

       The above approach can be optimized by observing the fact that the leaf nodes need not to be heapified as they already 
       follow the heap property. Also, the array representation of the complete binary tree contains the level order traversal 
       of the tree. So the idea is to find the position of the last non-leaf node and perform the heapify operation of each 
       non-leaf node in reverse level order.
    */
    void build_heap(const std::vector<std::shared_ptr<T>>& arr)
    {
        m_tree.clear();
        m_tree = arr;

        int last_non_leaf_index = m_tree.size()/2 - 1;
        for(size_t i = 1; i <= last_non_leaf_index; i++)
        {
            size_t cur_index = i;
            while(cur_index > 1 && compare(m_tree[this->getParentIndex()], m_tree[cur_index]))
            {
                std::swap(m_tree[this->getParentIndex()], m_tree[cur_index]);
                cur_index = this->getParentIndex();
            }
        }
    }

    void heapify()
    {
        for(size_t i = 1; i <= m_tree.size(); i++)
        {
            size_t cur_index = i;
            while(cur_index > 1 && compare(m_tree[this->getParentIndex()], m_tree[cur_index]))
            {
                std::swap(m_tree[this->getParentIndex()], m_tree[cur_index]);
                cur_index = this->getParentIndex();
            }
        }
    }

    bool compare(const T& parent, const T& child)
    {
        if(m_type == heapType::Max)
        {
            return parent > child;
        }
        else if(m_type == heapType::Min)
        {
            return parent < child;
        }
    }
};
