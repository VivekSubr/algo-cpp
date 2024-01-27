#pragma once
#include "binary_tree.h"

enum class heapType 
{
    Max,
    Min
};

template <typename T>
class Heap : public BinaryTree<T>
{
    using BinaryTree<T>::m_tree;

public:
    Heap(const std::vector<std::shared_ptr<T>>& arr, heapType type, bool isHeap = false): m_type(type), m_tree(arr)
    {
        if(!isHeap) heapify(arr);
    }

private:
    heapType m_type;

    void heapify(const std::vector<std::shared_ptr<T>>& arr)
    {
        m_tree.clear();
        for(size_t i=0; i<arr.size(); i++)
        { //create a BST from the data
            
        }
    }

    void swap()
    {
        
    }
};
