#pragma once
#include <string>

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

    int i_str(0), i_sub_str(0);
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


//leetcode.com/problems/longest-increasing-subsequence/description/
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
int lengthOfLIS(std::vector<int>& nums) 
{
    
}
