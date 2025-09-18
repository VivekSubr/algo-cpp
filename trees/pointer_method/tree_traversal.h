#pragma once
#include "binary_tree.h"

//leetcode.com/problems/binary-tree-zigzag-level-order-traversal
std::vector<std::vector<int>> zigzagLevelOrder(treeNode* root) 
{
    std::deque<treeNode*> Q;
    Q.push_back(root);

    int   level          = 0;
    int   level_sz       = std::pow(2, level); //for every level, we will have 2 power level nodes
    bool  left_to_right  = true;
    int   nodes_pushed   = 0;
    std::vector<int> curr_level;
    std::vector<std::vector<int>> ret;
    while(!Q.empty()) 
    {
        treeNode* node = Q.front(); Q.pop_front();
        if(nodes_pushed < level_sz) 
        {
            nodes_pushed   = 0;
            level_sz      = std::pow(2, ++level);
            left_to_right = !left_to_right;
            ret.push_back(curr_level);
            curr_level.clear();
        }
        
        if(left_to_right) {
            if(node->left)  Q.push_back(node->left.get());
            if(node->right) Q.push_back(node->right.get());
        } else {
            if(node->left)  Q.push_back(node->right.get());
            if(node->right) Q.push_back(node->left.get());
        }

        nodes_pushed = nodes_pushed + 2;
        curr_level.push_back(node->val);
    }

    return ret;
}