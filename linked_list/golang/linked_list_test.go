package linked_list

import (
    "testing"
    "github.com/stretchr/testify/assert"
)

// TestBasic tests basic linked list stuff
func TestBasic(t *testing.T) {
    val := []int{1, 2, 3, 4, 5 }
    listHead := CreateList(val)
 
    // Use assert to validate conditions
    assert.NotNil(t, listHead, "Expected list head to be non-nil")

    expectedValues := []int{1, 2, 3, 4, 5}
    currentNode := listHead

    for i, expectedValue := range expectedValues {
        assert.NotNil(t, currentNode, "Expected node at index %d to be non-nil", i)
        assert.Equal(t, expectedValue, currentNode.val, "Value mismatch at index %d", i)
        currentNode = currentNode.next
    }

    assert.Nil(t, currentNode, "Expected end of list, but found extra nodes")

    nList1 := CreateList([]int{1, 2, 3, 4, 5 })
    nList2 := CreateList([]int{1, 2, 3, 6 })

    assert.True(t,  AreEqual(listHead, nList1))
    assert.False(t, AreEqual(listHead,nList2))

    assert.Equal(t, listHead.Length(), uint(5))

    reversedVal := []int{5, 4, 3, 2, 1}
    currentNode  = listHead.Reverse()
    for i, expectedValue := range reversedVal {
        assert.NotNil(t, currentNode, "Expected node at index %d to be non-nil", i)
        assert.Equal(t, expectedValue, currentNode.val, "Value mismatch at index %d", i)
        currentNode = currentNode.next
    }
}

func TestCopyRandomList(t *testing.T) {
    
}