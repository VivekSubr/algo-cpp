#pragma once
#include <vector>
#include <algorithm> 
#include <iostream>

//https://en.wikipedia.org/wiki/B-tree
//https://www.youtube.com/watch?v=K1a2Bk8NrYQ

/* 
    Every node has n keys, and n + 1 children. Root node is exempt from this rule.

    So, each child lies between two parent keys, with one child left of smallest and one right of largest.

    *All* leaves need to be at same level.

    Each node has to have keys between a min and max number, Choose max number and min number = max/2.

    Insertion ->
    * Add to node in sorted order till we hit max.
    * Once max is hit, the next insertion triggers a node split. 
    * Create two nodes using half of the keys, leaving the parent with only the middle one (n/2, + 1 being inserted)
    * New keys are inserted at the bottom of the b-tree, traverse from root to find out where.
    * Note when splits are happening at non-root, the remaining parent key is pushed up rather than creating an intermediate node.
    * Intermediate nodes are created when the pushed parent causes node to overflow (exceed max number)
    
    Deletion ->
    * Traverse from root and delete key.
    * If deletion leaves a node with lesser than min keys then take keys from neighoring nodes at same level.
    * This re-arrangement forces changes in parent nodes to maintain left/right search property. 
      If required, child nodes and seperators in parent are merged together.
*/

template <typename T>
class BTree 
{
public:
    struct Node 
    {
        std::vector<T>                     keys;     //keys need to be sorted order
        std::vector<std::shared_ptr<Node>> children;

        bool isLeaf;

        Node(const T& value)
        {
            keys.push_back(value);
        }

        Node(std::vector<T>&& val) : keys(std::move(val))
        {
        }
    };

    BTree(int order): m_order(order)
    {
    }

    BTree(const std::vector<T>& values, int order): m_order(order)
    {
        for(auto val : values)
        {
            insert(std::move(val));
        }
    }

    void insert(const T&& value)
    {
        if(m_root)
        {
           traverse_insert(m_root, value);
        }
        else 
        {
            m_root = std::make_shared<Node>(value);
            m_root->isLeaf = true;
        }
    }

    void del(const T&& value)
    {
    }

    void printTree();
    void printLevel(std::shared_ptr<Node> node, int level);

    bool isValid();

private:
    std::size_t           m_order; //max numer of keys in a node. Min number is m_order/2
    std::shared_ptr<Node> m_root = nullptr;

    void traverse_insert(std::shared_ptr<Node> node, const T& value)
    {
        if(node->isLeaf)
        {
            for(size_t i=0; i<node->keys.size(); i++)
            {
                if(node->keys[i] > value)
                { //have to insert here.
                    node->keys.insert(std::upper_bound(node->keys.begin(), node->keys.end(), value), value);
                    rebalance(node);
                    return;
                }
            }

            node->keys.push_back(value);
            rebalance(node);
        }
        else 
        {
            for(size_t i=0; i<node->keys.size(); i++)
            {
                if(node->keys[i] > value)
                { 
                    traverse_insert(node->children[i], value);
                    return;
                }
            }

            traverse_insert(*node->children.end(), value);
        }
    }

    void rebalance(std::shared_ptr<Node> node)
    {
        if(node->keys.size() > m_order)
        {//split the nodes
        }
    }
};

template <typename T>
void BTree<T>::printTree()
{
    int level = 0;
    printLevel(m_root, level);
}

template <typename T>
void BTree<T>::printLevel(std::shared_ptr<BTree<T>::Node> node, int level)
{
    if(node)
    {
        std::cout<<"level "<<level<<'\n';
        for(auto key : node->keys)
        {
            std::cout<<key<<" ";
        } std::cout<<"\n\n";

        for(auto child : node->children)
        {
            printLevel(child, level + 1);
        }
    }
}

template <typename T>
bool BTree<T>::isValid()
{
    return false;
}