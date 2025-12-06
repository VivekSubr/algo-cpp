package linked_list

//https://leetcode.com/problems/copy-list-with-random-pointer/
type Node struct {
    Val int
    Next *Node
    Random *Node
}

func CopyRandomList(head *Node) *Node {
	NewHead := &Node{Val:head.Val, Next:nil, Random:nil}
	it    := head
	newIt := NewHead

	//assume the values are unique, maintain map of value to node
	valueMap := make(map[int]*Node)
	valueMap[head.Val] = NewHead

	//Traverse once creating new list, linked by Next pointers
	for it != nil {
		it = it.Next
		if it == nil { 
			break 
		}

		newNode := &Node{Val:it.Val, Next:nil, Random:nil}
		newIt.Next = newNode
		newIt = newIt.Next
	}

	it = head
	newIt = NewHead

	//Traverse again to set Random pointers
	for it != nil {
		if it.Random != nil {
			node := valueMap[it.Random.Val]
			newIt.Random = node
		}
		it    = it.Next
		newIt = newIt.Next
	}

	return NewHead
}