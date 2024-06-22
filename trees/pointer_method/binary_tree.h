#pragma once
#include <memory>
#include <vector>

struct treeNode 
{
    treeNode(int v): val(v)
    {
    }

    int val = 0;

    std::shared_ptr<treeNode> right = nullptr;
    std::shared_ptr<treeNode> left  = nullptr;
};

class tree 
{
public:
    tree(const std::vector<int>& treeArr)
    {
        if(treeArr.size() > 1)
        {
            if(treeArr == -1) throw std::runtime_error("root can't be null!");

            m_root = std::make_shared<treeNode>(treeArr[0]);

            buildTree(treeArr, m_root, getLeftChildIndex(0));
            buildTree(treeArr, m_root, getRightChildIndex(0));
        }
    }

    std::shared_ptr<treeNode> getRoot() { return m_root; }

private:
    std::shared_ptr<treeNode> m_root = nullptr;

    int getLeftChildIndex(int index) const
    {
        return 2 * index + 1;
    }

    int getRightChildIndex(int index) const
    {
        return 2 * index + 2;
    }

    void buildTree(const std::vector<int>& treeArr, std::shared_ptr<treeNode> node, int index)
    {
        if(index < treeArr.size() && treeArr[index] >= 0)
        {
            std::shared_ptr<treeNode> nNode = std::make_shared<treeNode>(treeArr[index]); 
            
            if(index % 2) node->left  = nNode;
            else          node->right = nNode;

            buildTree(treeArr, nNode, getLeftChildIndex(index));
            buildTree(treeArr, nNode, getRightChildIndex(index));
        }
    }
};