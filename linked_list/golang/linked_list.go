package linked_list

type ListNode struct {
	val   int 
	next *ListNode
}

func newListNode() *ListNode {
	return &ListNode{
		val:  0,
		next: nil,
	}
}

//note golang *doesn't* do function overloading
func newListNodeWithVal(v int) *ListNode {
	return &ListNode{
		val:  v,
		next: nil,
	}
}

func CreateList(values []int) *ListNode {
	head := newListNodeWithVal(values[0])
    node := head
	for i := 1; i < len(values); i++ {
		newNode := newListNodeWithVal(values[i])
		node.next = newNode
		node = newNode
	}

	return head
}

func AreEqual(list1, list2 *ListNode) bool {
	it1 := list1
	it2 := list2
	for it1 != nil && it2 != nil {
		if it1.val != it2.val {
			return false
		} 

		it1 = it1.next
		it2 = it2.next
	}

	return (it1 == nil && it2 == nil)
}

func (l *ListNode) Length() uint {
	len := uint(0)
	it := l;
	for it != nil {
		len++
		it = it.next
	}

	return len
}

func (l *ListNode) Reverse() *ListNode {
	cur := l
	var next, prev *ListNode
	for cur != nil {
		next     = cur.next
		cur.next = prev
		 
		prev     = cur 
		cur      = next
	}

	return prev
}