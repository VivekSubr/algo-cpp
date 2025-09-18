#pragma once

//leetcode.com/problems/two-sum-ii-input-array-is-sorted/
std::pair<int, int> two_sum(std::vector<int> &nums, int target)
{
    int start = 0, end = nums.size() - 1;
    while (start < end)
    {
        int sum = nums[start] + nums[end];
        if(sum == target) return {start + 1, end + 1};        
        else if (sum < target) start++;
        else                   end--;              
    }

    return {-1, -1};
}

//https://leetcode.com/problems/3sum/
/*
    Given an integer array nums, return all the triplets [nums[i], nums[j], nums[k]] 
    such that i != j, i != k, and j != k, and nums[i] + nums[j] + nums[k] == 0.

    Notice that the solution set must not contain duplicate triplets.
*/

std::set<std::tuple<int, int, int>> three_sum(std::vector<int>& nums) 
{
    std::sort(nums.begin(), nums.end());
    std::set<std::tuple<int, int, int>> ret;

    for(int i=0; i<nums.size(); i++)
    {
        int j = i + 1;
        int k = nums.size() - 1;
        while(j < k)
        {
            int sum = nums[i] + nums[j] + nums[k];
            if(sum == 0) ret.insert({nums[i], nums[j], nums[k]});
            
            j++;
            k--;
        }
    }

    return ret;
}

void max_area()
{

}