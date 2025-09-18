package trees

//https://leetcode.com/problems/flatten-binary-tree-to-linked-list/

type TreeNode struct {
    Val int
    Left *TreeNode
    Right *TreeNode
}

// Helper to collect nodes in preorder
func preOrder(node *TreeNode, order *[]*TreeNode) {
    if node == nil {
        return
    }
    *order = append(*order, node)
    preOrder(node.Left, order)
    preOrder(node.Right, order)
}

// Flattens the tree to a linked list in-place
func flatten(root *TreeNode) {
    var order []*TreeNode
    preOrder(root, &order)
    for i := 0; i < len(order)-1; i++ {
        order[i].Left = nil
        order[i].Right = order[i+1]
    }
    if len(order) > 0 {
        order[len(order)-1].Left = nil
        order[len(order)-1].Right = nil
    }
}