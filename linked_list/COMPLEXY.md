Linked List complexity tends to be O(n), all the functions in linked_list.h are O(n).

## Merge 2 Sorted
Basically, create a new list with pointers pointing to old lists... O(n + m)... O(n) complexity. 

## Reversing a List
Basically, 
1 --> 2 --> 3 --> 4 

Becomes 
4 --> 3 --> 2 --> 1

```
    ListNode* prev = head;
    ListNode* node = head->next; 
    while(node != nullptr)
    {
        ListNode* next = node->next;
        node->next     = prev;

        node = next;
    }
```

This basically works unless head is nullptr, so if(head == nullptr) return;


The key thing is being able to mentally walk what the code is doing with the nodes. Let's debug this attempt at reverse between left and right,
```
ListNodePtr reverse(ListNodePtr node, int nCount)
{
   ListNodePtr prev = nullptr;
   ListNodePtr ret  = node;
   for(int i=0; i<nCount; i++)
   {
      ListNodePtr next = node->next;
      node->next = prev;
      prev       = node;
      node       = next;
   }

   return ret;
}

ListNodePtr reverse_between(ListNodePtr head, int left, int right) 
{
    int nNode(0);

    ListNodePtr prev(nullptr);
    ListNodePtr node(head);

    while(node != nullptr)
    {
       if(nNode >= left && nNode <= right)
       {
	        ListNodePtr ret = reverse(node, right - nNode);
          ret->next = prev;
	        break;
       }

       prev = node;
       node = node->next;

       ++nNode;
    }

    return head;
}
```

So, we are tracking previous node, so the loop is 

prev = null
node = head

1st iteration
  prev = head
  node = 2nd 
  nNode = 1

2st iteration 
  prev = 2nd
  node = 3rd
  nNode = 2

And so on, when we go inside the if, let's consider the gtest test, {1, 2, 3, 4, 5}, l=2, r=4

if hits when nNode = 2, ie 3rd iteration... prev = 2, node = 3, nCount = 4 - 2 = 2.
Hence, we start with prev = null, node = 3 and start running the loop.

i=0 --> 3 -> 2, prev = 3, node = 4
i=1 --> 4 -> 3, prev = 4, node = 5

return 3, with list now looking like this: 1 -> 2 <- 3 < - 4 -> 5... messed up!

What reverse needs to do is this: 1 -> 4 -> 3 -> 2 -> 5... look at in_place_op.h for the fixed code.

## Fast and Slow Pointer Method
The idea is this -> move a pointer on list one a time, and another 2 at time... if there is a cycle, the fast pointer will eventually meet or overlap the slow one.

