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

/*
Alternatively, using hashmap
std::pair<int, int> twoSum(std::vector<int>& nums, int target) 
{ 
    std::unordered_map<int, int> seen; // value -> index
    for(int i=0; i< nums.size(); i++) seen.insert({nums[i], i});

    for (int i = 0; i < nums.size(); i++) 
    {
        int complement = target - nums[i]; //what number we need
        auto it = seen.find(complement); //is that in map?
        if (it != seen.end()) 
        {
            return {it->second, i};
        }
        seen[nums[i]] = i;
    }
    return {}; // no pair found
}
*/


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

    for(std::size_t i=0; i<nums.size(); i++)
    {
        int j = i + 1;
        int k = nums.size() - 1;
        while(j < k)
        {
            int sum = nums[i] + nums[j] + nums[k];
            if(sum == 0) ret.insert({nums[i], nums[j], nums[k]});

            if(sum < 0) j++;
            else        k--;
        }
    }

    return ret;
}

/*
    Alternatively, using hashmap
    std::set<std::tuple<int, int, int>> three_sum(std::vector<int>& nums) 
    {
        std::unordered_map<int, int> seen; // value -> index
        for(int i=0; i< nums.size(); i++) seen.insert({nums[i], i});

        std::set<std::tuple<int, int, int>> ret;

        for(std::size_t i=0; i<nums.size(); i++)
        {
            for(std::size_t j = i + 1; j < nums.size(); j++)
            {
                int complement = -(nums[i] + nums[j]);
                auto it = seen.find(complement);
                if(it != seen.end() && it->second != i && it->second != j)
                {
                    ret.insert({nums[i], nums[j], complement});
                }
            }
        }

        return ret;
    }

*/

void max_area()
{

}