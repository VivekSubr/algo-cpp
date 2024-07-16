#pragma once
#include <memory>
#include <vector>
#include <deque>
#include <stdexcept>
#include <cmath>

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
            if(treeArr[0] == -1) throw std::runtime_error("root can't be null!");

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

std::shared_ptr<treeNode> do_createBST(const std::vector<int>& arr, int start, int end)
{
    if(start > end) return nullptr;

    //Since array is sorted, and all elements to left of a node must be smaller, and right larger in a BST... 
    //we must pivot around midpoint of array.
    int mid = std::floor((start + end)/2);

    auto node = std::make_shared<treeNode>(arr[mid]);
    if(start == end) return node;

    node->left  = do_createBST(arr, 0, mid-1);
    node->right = do_createBST(arr, mid+1, end);
    return node;
}

//leetcode.com/problems/convert-sorted-array-to-binary-search-tree/
std::shared_ptr<treeNode> createBST(const std::vector<int>& arr)
{
    return do_createBST(arr, 0, arr.size() - 1);
}

//do a level order traversal and check if conditions are satisfied in each level
bool isValidBST(std::shared_ptr<treeNode> root)
{
    std::deque<std::shared_ptr<treeNode>> Q;
    Q.push_back(root);
    while(!Q.empty())
    {
        auto node = Q.front(); Q.pop_front();

        if(node->left)
        {
            if(node->left->val > node->val) return false;
            Q.push_back(node->left);
        }

        if(node->right)
        {
            if(node->left->val < node->val) return false;
            Q.push_back(node->left);
        }
    }

    return true;
}