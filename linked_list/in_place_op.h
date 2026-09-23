#include "linked_list.h"

//leetcode.com/problems/swap-nodes-in-pairs/description/
//leetcode.com/problems/reverse-linked-list-ii/description/
//leetcode.com/problems/reverse-nodes-in-k-group/description/

std::pair<ListNodePtr, ListNodePtr> reverse(ListNodePtr node, int nCount)
{
   ListNodePtr prev = nullptr;
   for(int i=0; i<nCount; i++)
   {
      ListNodePtr next = node->next;
      node->next = prev;
      prev       = node;
      node       = next;
   }

   return {prev, node};
}

ListNodePtr reverse_between(ListNodePtr head, int left, int right) 
{
   int nNode(1); //1 indexed node counter

   ListNodePtr prev(nullptr);
   ListNodePtr node(head);

   while(node != nullptr)
   {
      if(nNode >= left && nNode <= right)
      {
	      auto [new_head, ret] = reverse(node, right - nNode + 1);
         
         //List at this point is disjoint, with node being the new head of the reversed segment,
         //and ret being the last node
         
         node->next = ret; // Connect the last node of the reversed segment to the next part of the list
         prev->next     = new_head; // Connect the previous part of the list to the new head of the reversed segment
	      break;
      }

      prev = node;
      node = node->next;

      ++nNode;
   }

   return head;
}