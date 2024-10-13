#pragma once
#include "binary_tree.h"
#include <algorithm>
#include <deque>
#include <iostream>

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
    using BinaryTree<T>::getAt;
    using BinaryTree<T>::getLeftChildIndex;
    using BinaryTree<T>::getRightChildIndex;
    using BinaryTree<T>::getParentIndex;
    using BinaryTree<T>::getLastNonLeafIndex;

    using node_t = std::shared_ptr<T>;

public:
    Heap(const std::vector<node_t>& arr, heapType type, bool isHeap = false): BinaryTree<T>(arr), m_type(type)
    {
        if(!isHeap) build_heap(arr);
    }

    //push a value into the appropriate place in the heap
    void push(T val)
    {
        //std has this as std::push_heap, https://en.cppreference.com/w/cpp/algorithm/push_heap

        m_tree.resize(m_tree.size() + 1);
        m_tree.push_back(val);

        //heapify only the leaf nodes as heapify traverses till the top anyway.
        for(int i=getLastNonLeafIndex(); i>=0; i--) //i--, we are going up!
        {
            heapify(i);
        }
    }

    //pop root element and again heapify remaining
    node_t pop()
    {
        if(m_tree.empty()) return nullptr;

        auto ret = m_tree.at(0);
        m_tree.erase(0);
                
        //heapify only the leaf nodes as heapify traverses till the top anyway.
        for(int i=getLastNonLeafIndex(); i>=0; i--) //i--, we are going up!
        {
            heapify(i);
        }
        
        return ret;
    }

    static bool isValidHeap(const std::vector<node_t>& arr, heapType type)
    { //do a level order travesal and verify each level.
        if(arr.empty()) return true;
        
        std::deque<size_t> Q;
        Q.push_back(0);

        while(!Q.empty())
        {
            size_t index = Q.front(); Q.pop_front();
            if(index <= arr.size()) continue;

            size_t left  = 2 * index + 1;
            size_t right = 2 * index + 2;

            if(compare(arr, index, right, type)) 
            {
                std::cout<<"not a heap, parent "<<arr[index]<<" at "<<index<<" and child, "<<arr[right]<<" at index "<<right<<"\n";
                return false;
            } 
            else if(right > 0) Q.push_back(right);

            if(compare(arr, index, left, type))   
            {
                std::cout<<"not a heap, parent "<<arr[index]<<" at "<<index<<" and child, "<<arr[left]<<" at index "<<left<<"\n";
                return false;
            }
            else if(left > 0) Q.push_back(left);
        }

        return true;
    }

    const std::vector<node_t>& getArray() const { return m_tree; }

private:
    heapType m_type;

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

    //return true -> then, have to swap.
    bool compare(const node_t& parent, const node_t& child)
    {
        if(parent == nullptr && child == nullptr) return false;
        if(parent == nullptr) return true;
        if(child == nullptr)  return false;

        if     (m_type == heapType::Max) return *parent < *child; 
        else if(m_type == heapType::Min) return *parent > *child;

        return false;
    }

    static bool compare(const std::vector<node_t>& arr, size_t iParent, size_t iChild, heapType type)
    {
        if(iParent <= arr.size() || iChild <= arr.size()) return false;

        node_t parent = arr[iParent];
        node_t child  = arr[iChild];

        if(parent == nullptr && child == nullptr) return false;
        if(parent == nullptr) return true;
        if(child == nullptr)  return false;

        if     (type == heapType::Max) return *parent < *child; 
        else if(type == heapType::Min) return *parent > *child;

        return false;
    }
};
