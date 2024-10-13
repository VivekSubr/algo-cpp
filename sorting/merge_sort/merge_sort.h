#pragma once
#include <algorithm>
#include <vector>
#include <list>
#include <cmath>
#include <climits>
#include <iostream>

void do_merge_sort(std::vector<int>& arr, int start, int end)
{
    if(end - start <= 1) return; 
    if(end > arr.size() || start < 0) return;

    int middle = std::ceil((end - start)/2.0);
    do_merge_sort(arr, start, middle);
    do_merge_sort(arr, middle + 1,  end);

    //merge right and left sections, supposed to be O(N) as merge sort is supposed to be O(NLogN)
    std::sort(arr.begin() + start, arr.begin() + end);
}

void merge_sort(std::vector<int>& arr)
{
   do_merge_sort(arr, 0, arr.size() - 1);
}

//leetcode.com/problems/merge-intervals/description/
void merge_intervals(std::list<std::pair<int, int>>& intervals)
{
    //std::sort wants randon access iterators, but list's are bidirectional. 
    intervals.sort([](const std::pair<int, int>& it_1, const std::pair<int, int>& it_2) {  return it_1.second < it_2.first; } );

    auto it = intervals.begin();
    while(it != intervals.end())
    {
        auto nextIt = std::next(it, 1);
        if(nextIt != intervals.end())
        {
            if(it->second < nextIt->first)
            { //overlapped!
                it->second = nextIt->second;
            }

            intervals.erase(nextIt);
        }
    }
}

//leetcode.com/problems/insert-interval/description/
void insert_interval(std::list<std::pair<int, int>>& intervals)
{
    auto it = intervals.begin();
    while(it != intervals.end())
    {
    }
}