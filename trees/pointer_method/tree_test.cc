#include "binary_tree.h"
#include "tree_traversal.h"
#include "tree_test.h"

TEST_F(TestTree, ZigZag)
{
    std::shared_ptr<treeNode> root = std::make_shared<treeNode>(3);
    root->left  = std::make_shared<treeNode>(9);
    root->right = std::make_shared<treeNode>(20);       
    root->right->left  = std::make_shared<treeNode>(15);
    root->right->right = std::make_shared<treeNode>(7);

    auto ret = zigzagLevelOrder(root.get());
    std::vector<std::vector<int>> expected = {{3}, {20, 9}, {15, 7}};
    EXPECT_EQ(ret, expected);
}