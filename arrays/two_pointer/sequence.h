#pragma once
#include <string>
#include <vector>

bool is_sub_sequence(const std::string& str, const std::string& sub_str)
{
    if(sub_str.size() == 0) return true;
    if(sub_str.size() == 1)
    {
        char ch = sub_str.at(0);
        for(auto ele : str)
        {
            if(ele == ch) return true;
        }

        return false;
    }

    std::size_t i_str(0), i_sub_str(0);
    bool check_consecutive = false;
    while(i_str < str.size() && i_sub_str < sub_str.size())
    {
        if(str.at(i_str) == sub_str.at(i_sub_str))
        {
            check_consecutive = true;
            ++i_sub_str;
        }
        else if(check_consecutive)
        {
            i_sub_str = 0;
            check_consecutive = false;
        }
        else check_consecutive = false;

        if(i_sub_str == sub_str.size() - 1) return true;

        ++i_str;
    }

    return false;
}

size_t lengthOfLIS_continous(std::vector<int>& nums) 
{
    size_t start(0), end(0);
    size_t max_len(1);
    for(size_t i=1; i<nums.size(); i++)
    {
        if(nums[i] > nums[end]) end = i;
        else 
        {
            start = i;
            end = i;
        }

        max_len = std::max(max_len, end - start + 1);
        //std::cout<<"start: "<<start<<", end: "<<end<<" len "<<max_len<<"\n";
    }

    return max_len;
}



//leetcode.com/problems/longest-increasing-subsequence/description/
//Given an integer array nums, return the length of the longest strictly increasing subsequence
/*
approach:
Consider the example nums = [2, 6, 8, 3, 4, 5, 1], 
let's try to build the increasing subsequences starting with an empty one: sub1 = [].
Let pick the first element, sub1 = [2].
6 is greater than previous number, sub1 = [2, 6]
8 is greater than previous number, sub1 = [2, 6, 8]
3 is less than previous number, we can't extend the subsequence sub1, but we must keep 3 because in the future there may have the longest subsequence start with [2, 3], sub1 = [2, 6, 8], sub2 = [2, 3].
With 4, we can't extend sub1, but we can extend sub2, so sub1 = [2, 6, 8], sub2 = [2, 3, 4].
With 5, we can't extend sub1, but we can extend sub2, so sub1 = [2, 6, 8], sub2 = [2, 3, 4, 5].
With 1, we can't extend neighter sub1 nor sub2, but we need to keep 1, so sub1 = [2, 6, 8], sub2 = [2, 3, 4, 5], sub3 = [1].
Finally, length of longest increase subsequence = len(sub2) = 4.
*/

void branch_LIS(std::vector<int>& nums, std::size_t index, std::vector<std::vector<int>>& alternate_subseqs)
{
    std::vector<int> new_subseq;
    for(size_t i=0; i<nums.size(); i++)
    {
        if(nums[i] > nums[index] && i < index)
        { //we continue with this element
            i = index;
            new_subseq.push_back(nums[i]);
        }
        else if(nums[i] > new_subseq.back()) 
        {
            new_subseq.push_back(nums[i]);
        }
        else 
        { //again, we have branching choices
           branch_LIS(nums, i, alternate_subseqs);
        }
    }

    alternate_subseqs.push_back(new_subseq);
}

size_t lengthOfLIS(std::vector<int>& nums) 
{
    std::vector<int> subseq;
    std::vector<std::vector<int>> alternate_subseqs;
    for(size_t i=0; i<nums.size(); i++)
    {
        int ele = nums[i];
        if(subseq.empty() || ele > subseq.back()) subseq.push_back(ele);
        else
        {//now we have branching choices!
            branch_LIS(nums, i, alternate_subseqs);
        }
    }

    alternate_subseqs.push_back(subseq);

    auto it = std::max_element(alternate_subseqs.begin(), alternate_subseqs.end(),
         [](const std::vector<int>& a, const std::vector<int>& b)
         {
            return a.size() < b.size();   // compare by size
        });

    return it != alternate_subseqs.end() ? it->size() : 0;
}
