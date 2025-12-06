#pragma once
#include <vector>
#include <climits>

//leetcode.com/problems/range-sum-query-immutable/description/
class NumArray 
{
public:
    NumArray(std::vector<int>& nums) 
    {
        m_prefix_sum.resize(nums.size() + 1);
        for(size_t i = 0; i < nums.size(); ++i) 
        {
            m_prefix_sum[i + 1] = m_prefix_sum[i] + nums[i];
        }
    }
    
    int sumRange(int left, int right) 
    {
        //sum of elements from left to right = sum(0 to right) - sum(0 to left-1)
        //eg: {1,2,3,4,5,6,7,8,9}, sum(2,5) = sum(0,5) - sum(0,1) = (1+2+3+4+5+6) - (1+2) = 3+4+5+6 = 18
        return m_prefix_sum[right + 1] - m_prefix_sum[left];
    }

private:
    //every element at i stores sum of elements from 0 to i-1
    std::vector<int> m_prefix_sum;
};

//leetcode.com/problems/subarray-sum-equals-k/description/
//given an array of integers nums and an integer k, return the total number of subarrays whose sum equals to k.
int subarraySum(std::vector<int>& nums, int k) 
{
    return 0;
}