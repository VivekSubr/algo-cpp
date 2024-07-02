#pragma once
#include <unordered_set>
#include <vector>
#include <memory>
#include <stdexcept>
#include <iostream>

//Catalan numbers are a sequence that is often useful in solving combination problems.

//https://brilliant.org/wiki/catalan-numbers/
//https://www.reddit.com/r/explainlikeimfive/comments/ei810z/eli5_what_do_the_catalan_numbers_represent_and/
//https://www.youtube.com/watch?v=fczN0BCx0xs

std::size_t get_nth_catalan(std::size_t n)
{
    if(n <= 1) return 1;
 
    // catalan(n) is sum of catalan(i)*catalan(n-i-1)
    std::size_t res = 0;
    for(int i = 0; i < n; i++) 
    {
        res += get_nth_catalan(i) * get_nth_catalan(n - i - 1);
    }
 
    return res;
}


//Catalan numbers can give count of binary trees possible to be made with n nodes... following is a class to actually print them.
//https://leetcode.com/problems/unique-binary-search-trees-ii/solutions/1849266/c-detailed-explanation-recursive-tree-with-comments/
class UniqBinaryTrees 
{
    struct TreeNode 
    {
       int val;
       std::shared_ptr<TreeNode> left;
       std::shared_ptr<TreeNode> right;
       TreeNode() : val(0), left(nullptr), right(nullptr) {}
       TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
       TreeNode(int x, std::shared_ptr<TreeNode> left, std::shared_ptr<TreeNode> right) : val(x), left(left), right(right) {}
    };

public:
    static std::vector<std::shared_ptr<TreeNode>> uniqueBinaryTrees(int nNodes)
    {
        return do_uniqBinaryTrees(1, nNodes);
    }

private:
    //Actually, for BSTs. This is a bottom up approach.
    static std::vector<std::shared_ptr<TreeNode>> do_uniqBinaryTrees(int start, int end)
    {
        std::vector<std::shared_ptr<TreeNode>> ret;
        if(start > end) { 
            ret.push_back(nullptr); //Push back a null node
            return ret;
        }

        // Iterate through all values from start to end to construct left and right subtree recursively
	    for(int i = start; i <= end; ++i) 
        {
            //For a given i, all values < i lie in the left subtree, and > than i on the right subtree.
            std::vector<std::shared_ptr<TreeNode>> leftSubTree  = do_uniqBinaryTrees(start, i - 1);    // Construct left subtree
            std::vector<std::shared_ptr<TreeNode>> rightSubTree = do_uniqBinaryTrees(i + 1,   end);    // Construct right subtree

            // loop through all left and right subtrees and connect them to ith root  
		    for(int j = 0; j < leftSubTree.size(); j++) 
            {
			    for(int k = 0; k < rightSubTree.size(); k++) 
                {
                    auto nRoot   = std::make_shared<TreeNode>(i); //this will be leaf if last returns were nullptr.
                    nRoot->left  = leftSubTree[j];
                    nRoot->right = rightSubTree[k];

                    ret.push_back(nRoot);
                }
            }
        }

        return ret;
    }

    static void printTree(TreeNode* root)
    {
    }
};