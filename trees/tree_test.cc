#include "tree_traversal.h"
#define private public
    #include "bst.h"   
    #include "heap.h"
    #include "b_tree.h"
#undef private
#include "tree_test.h"
#include <gmock/gmock-matchers.h>
using namespace testing;

#define make_int(x) std::make_shared<int>(x)

TEST_F(TestTree, BinaryTree)
{
    BinaryTree<int> tree(arr2pts({1, 2, 3, 4, 5, 6, 7, 8, -1, -1, -1, 9, 10}));
    tree.printTree();
    
    //              1
    //       2               3
    //  4       5       6        7
    //  8           9     10

    ASSERT_EQ(*tree.getRoot(), 1);

    auto right = tree.getRightChild(0);
    auto left  = tree.getLeftChild(0);

    ASSERT_EQ(left.first, 1);
    ASSERT_EQ(*left.second, 2);
    ASSERT_EQ(right.first, 2);
    ASSERT_EQ(*right.second, 3);

    right = tree.getRightChild(1);
    left  = tree.getLeftChild(1);

    ASSERT_EQ(left.first, 3);
    ASSERT_EQ(*left.second, 4);
    ASSERT_EQ(right.first, 4);
    ASSERT_EQ(*right.second, 5);

    right = tree.getRightChild(3);
    left  = tree.getLeftChild(3);

    ASSERT_EQ(*left.second, 8);
    ASSERT_EQ(right.second, nullptr);
}

TEST_F(TestTree, BinaryTreeTraversal)
{
    BinaryTree<int> tree({make_int(1), make_int(2), make_int(3), make_int(4), make_int(5), make_int(6), make_int(7), 
                          make_int(8), nullptr, nullptr, nullptr, make_int(9), make_int(10)});

    ASSERT_EQ(tree.getMaxIndex(), 12);
    
    auto preOrder = preOrderTraversal<int>(tree);
    std::vector<int> preOrderEx = {1, 2, 4, 8, 5, 3, 6, 9, 10, 7};
    ASSERT_EQ(preOrder.size(), preOrderEx.size());
    for(std::size_t i=0; i<preOrder.size(); i++) { ASSERT_EQ(*preOrder[i], preOrderEx[i]); }

    auto postOrder = postOrderTraversal<int>(tree);
    std::vector<int> postOrderEx = {8, 4, 5, 2, 9, 10, 6, 7, 3, 1};
    ASSERT_EQ(postOrder.size(), postOrderEx.size());
    for(std::size_t i=0; i<postOrder.size(); i++) { ASSERT_EQ(*postOrder[i], postOrderEx[i]); }

    auto inOrder = inOrderTraversal<int>(tree);
    std::vector<int> inOrderEx = {8, 4, 2, 5, 1, 9, 6, 10, 3, 7};
    ASSERT_EQ(inOrder.size(), inOrderEx.size());
    for(std::size_t i=0; i<inOrder.size(); i++) { ASSERT_EQ(*inOrder[i], inOrderEx[i]); }

    auto levelOrder = levelOrderTraversal<int>(tree);
    std::vector<int> levelOrderEx = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    ASSERT_EQ(levelOrder.size(), levelOrderEx.size());
    for(std::size_t i=0; i<levelOrder.size(); i++) { ASSERT_EQ(*levelOrder[i], levelOrderEx[i]); }
}

TEST_F(TestTree, BST)
{
    auto make_invalid_tree = []() { 
        BinarySearchTree<int> bst_invalid({make_int(1), make_int(3), make_int(8), make_int(4)}); 
    };
    ASSERT_THROW(make_invalid_tree(), std::runtime_error);

    //Valid BST
    BinarySearchTree<int> bst({make_int(8), make_int(3), make_int(10), make_int(1), make_int(6), nullptr, make_int(14),
                         nullptr, nullptr, make_int(4), make_int(7), make_int(13)});
    bst.printTree();

    std::cout<<"***************\n";
    bst.insert(make_int(2));
    ASSERT_TRUE(bst.checkIfValid());
    bst.printTree();
}

TEST_F(TestTree, Heap)
{
    Heap<int> heap({make_int(10), make_int(100), make_int(40), make_int(15), make_int(30), make_int(50), make_int(40), nullptr}, heapType::Max);

    heap.printTree();
    ASSERT_TRUE(Heap<int>::isValidHeap(heap.getArray(), heapType::Max));
}

TEST_F(TestTree, BTree)
{
    std::vector<int> arr = {100, 35, 65, 130, 180, 10, 20, 40, 50, 70, 80, 90, 110, 120, 140, 160, 190, 240, 260};

    BTree<int> tree(arr, 3);
    tree.printTree();
}