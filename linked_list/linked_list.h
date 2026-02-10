#pragma once
#include <memory>
#include <sstream>
#include <string>
#include <vector>

struct ListNode 
{
    int val;
    std::shared_ptr<ListNode> next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, std::shared_ptr<ListNode> next) : val(x), next(next) {}
};

std::shared_ptr<ListNode> createList(const std::vector<int>& arr)
{
    std::shared_ptr<ListNode> head = std::make_shared<ListNode>(arr[0]);
    std::shared_ptr<ListNode> list = head;
    for(size_t i=1; i<arr.size(); i++)
    {
        list->next = std::make_shared<ListNode>(arr[i]);
        list = list->next;
    }

    return head;
}

bool isEqual(std::shared_ptr<ListNode> list1, std::shared_ptr<ListNode> list2)
{
    while(list1 != nullptr && list2 != nullptr)
    {
        if(list1->val != list2->val) return false;

        list1 = list1->next;
        list2 = list2->next;
    }

    return list1 == nullptr && list2 == nullptr;
}

int length(std::shared_ptr<ListNode> list)
{
    int ret = 0;
    while(list != nullptr)
    {
        ++ret;
        list = list->next;
    }

    return ret;
}

void reverse(std::shared_ptr<ListNode>& list) 
{
    std::shared_ptr<ListNode> prev    = nullptr;
    std::shared_ptr<ListNode> current = list;
    std::shared_ptr<ListNode> next    = nullptr;

    while (current != nullptr) 
    {
        next = current->next;
        current->next = prev;

        prev = current;
        current = next;
    }

    list = prev;
}

std::string print_list(std::shared_ptr<ListNode> list)
{
    std::ostringstream oss;
    while(list != nullptr)
    {
        oss << list->val << " ";
        list = list->next;
    }
    return oss.str();
}