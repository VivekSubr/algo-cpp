#pragma once
#include <vector>
#include <climits>
#include <deque>

//https://leetcode.com/problems/sliding-window-maximum/description/
//Max is max sum of elements in sliding window of size k
std::vector<int> maxSlidingWindow(std::vector<int>& nums, std::size_t k) 
{
/*
    This O(n^2) code works, but not what question wants,
    
    std::vector<int> ret;
    for(int i=0; i<nums.size()- k + 1; i++)
    {
        int max = INT_MIN;
        for(int j=i; j<i+k && j<nums.size(); j++)
        {
            if(nums[j] > max) max = nums[j];
        }
        ret.push_back(max);
    }

    return ret;
*/

/* 
   Another idea - BST, very time we slide the window, we remove the node that's out of the window 
   and add the one that comes into the window to the tree
*/



/* 
  In the brute force solution, notice the repeated work being done - each time we slide the window by 1, we are 
  re-evaluating the entire window, where most of the elements are common between the two instances. 

  So, key observation is that, when sliding the window, only two elements are changed, one is removed from the left side, 
  one is added from the right side... hence, we can represent the window as a priority queue, pushing and poping as required.

  now, ask is to get max element from each window... whenever we push an element, we pop all the smaller elements before it!
  When we slide the window, we check if element is still in the window, if it is, pop it.

  neetcode: https://www.youtube.com/watch?v=DfljaUwZsOk

  Example: [8, 7, 6, 9], k=2
    i=0, dq = [8]
    i=1, dq = [8, 7] -> 8 not popped since 8 > 7
    i=2, dq = [7, 6] -> pop 8, since out of window, ret = [8]
    i=3, dq = [9]    -> pop 7 since out of window, pop 6 since smaller than 9, ret = [8,7,9]
*/

    std::vector<int>        ret;
    std::deque<std::size_t> dq; //store the index of elements in nums, the elements are in decreasing order
    
    std::size_t left(0), right(0);
    while(right < nums.size())
    {
        //first, pop elements out of window
        if(!dq.empty() && dq.front() < left) 
        {
            ret.push_back(nums[dq.front()]);
            dq.pop_front();
        }

        //now, pop all elements smaller than the incoming element
        while(!dq.empty() && nums[dq.front()] < nums[right]) 
        { 
            dq.pop_front();
            ret.push_back(nums[dq.front()]); 
        }

        //add to dq
        dq.push_front(right);

        right++;
        left = right - k;
    }

    return ret;
}
