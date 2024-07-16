#pragma once
#include "linked_list.h"

std::shared_ptr<ListNode> merge_k_sorted(std::vector<std::shared_ptr<ListNode>>& lists)
{
    
}

std::shared_ptr<ListNode> merge_2_sorted(std::shared_ptr<ListNode> list1, std::shared_ptr<ListNode> list2)
{
    auto it_1 = list1;
    auto it_2 = list2;

    std::shared_ptr<ListNode> ret  = nullptr;
    if(it_1 != nullptr && it_2 != nullptr)
    {
        if(it_1->val < it_2->val)
        {
            ret = it_1;
            it_1 = it_1->next;
        }
        else
        {
            ret  = it_2;
            it_2 = it_2->next;
        }
    }
    else if(it_1 != nullptr)
    {
        ret  = it_1;
        it_1 = it_1->next;
    }
    else if(it_2 != nullptr)
    {
        ret  = it_2;
        it_2 = it_2->next;
    }
    else return nullptr;

    std::shared_ptr<ListNode> n_it = ret;
    while(n_it != nullptr && (it_1 != nullptr || it_1 != nullptr))
    {
        if(it_1 != nullptr && it_2 != nullptr)
        {
            if(it_1->val < it_2->val)
            {
                n_it->next = it_1;
                it_1 = it_1->next;
            }
            else
            {
                n_it->next = it_2;
                it_2 = it_2->next;
            }
        }
        else if(it_1 != nullptr)
        {
            n_it->next = it_1;
            it_1 = it_1->next;
        }
        else if(it_2 != nullptr)
        {
            n_it->next = it_2;
            it_2 = it_2->next;
        }

        n_it = n_it->next;
    }

    return ret;
}
