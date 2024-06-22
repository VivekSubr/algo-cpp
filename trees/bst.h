#pragma once
#include "binary_tree.h"
#include <deque>

template <typename T>
class BinarySearchTree : public BinaryTree<T>
{
    //This is a wart with template inheritance
    //https://www.modernescpp.com/index.php/surprise-included-inheritance-and-member-functions-of-class-templates/
    using BinaryTree<T>::m_tree;
    using BinaryTree<T>::getAt;
    using BinaryTree<T>::getLeftChild;
    using BinaryTree<T>::getRightChild;

public:
    BinarySearchTree(const std::vector<std::shared_ptr<T>>& arr): BinaryTree<T>(arr)
    {
        if(!checkIfValid()) throw std::runtime_error("Invalid BST");
    }

    int insert(std::shared_ptr<T> node)
    {
        return traverse_insert(node, 0);
    }

    bool del(std::shared_ptr<T> node)
    {
        return traverse_delete(node, 0);
    }

    int has(std::shared_ptr<T> node)
    {
        return -1;
    }

private:
    /*
       Here, we are traversing the BST from root to the point where new node can be inserted. 
       If this insertion is displacing an existing node, the subtree at that point needs to be *rebalanced*
    */
    int traverse_insert(std::shared_ptr<T> new_node, int cur_index)
    {
        auto node  = getAt(cur_index);
        auto left  = getLeftChild(cur_index);
        auto right = getRightChild(cur_index);

        if(node == nullptr)
        { //insert here
            m_tree[cur_index] = new_node;
            return cur_index;
        }

        if(*new_node < *node)
        {//go left
            if(left.first < 0) 
            { //null, hence insert here
                m_tree[left.first] = new_node;
                return left.first;
            }

            if(*new_node < *left.second) 
            { //traverse further
                return traverse_insert(new_node, left.first);
            }
            else 
            { //insert at this point 
                rebalance(new_node, left.first);
                return left.first;
            } 
        }
        else //go right
        {
            if(right.first < 0) 
            {
                m_tree[right.first] = new_node;
                return right.first;
            }

            if(*new_node > *right.second) 
            { //traverse further
                return traverse_insert(new_node, right.first);
            }
            else 
            { //insert at this point 
                rebalance(new_node, right.first);
                return right.first;
            }
        }
    }

    bool traverse_delete(std::shared_ptr<T> del_node, int cur_index)
    {
        auto node  = getAt(cur_index);
        auto left  = getLeftChild(cur_index);
        auto right = getRightChild(cur_index);

        if(node == nullptr) return false;

        if(*del_node == *node)
        { 
            rebalance(cur_index);
            return true;
        }

    }

    /*
        Rebalancing after insertion.
    */
    void rebalance(std::shared_ptr<T> new_node, int index)
    {
        auto old_node =  m_tree.at(index);
        m_tree.at(index) = new_node; 

        if(*old_node < *new_node)
        {
            auto left = getLeftChild(index);
            if(left.second == nullptr)
            {
               m_tree.at(left.first) = old_node; 
               return;
            }

            rebalance(old_node, left.first);
        }
        else
        {
            auto right = getRightChild(index);
            if(right.second == nullptr)
            {
               m_tree.at(right.first) = old_node; 
               return;
            }

            rebalance(old_node, right.first);
        }
    }

    /*
        Rebalance after deletion.
    */
    void rebalance(int index)
    {

    }

    bool checkIfValid()
    {
        std::deque<int> Q;
        Q.push_back(0);
        while(!Q.empty())
        {
            int node  = Q.front(); Q.pop_front();
            if(getAt(node) == nullptr) continue;
            
            int node_val = *getAt(node);

            auto left = getLeftChild(node);
            if(left.second)
            {
                if(node_val < *left.second) return not_valid_error(node_val, *left.second);
                Q.push_back(left.first);
            }

            auto right = getRightChild(node);
            if(right.second)
            {
                if(node_val > *right.second) return not_valid_error(node_val, *right.second);
                Q.push_back(right.first);
            }

            if(left.second && right.second && *left.second > *right.second)
            {
                return not_valid_error(*left.second, *right.second);
            }
        }

        return true;
    }

    bool not_valid_error(T first, T second)
    {
        std::cout<<first<<" and "<<second<<" not in correct order\n"; 
        this->printTree();
        std::cout<<"\n";
        return false;
    }
};