#pragma once
#include <vector>
#include <algorithm>

//https://leetcode.com/problems/jump-game/description/
//https://leetcode.com/problems/jump-game-ii/description/

class JumpGame
{
public:
    static bool canJump(const std::vector<int>& nums)
    {
        return jump(nums, 0);
    }

    static int minJumps(const std::vector<int>& nums)
    {
        return jump(nums, 0, 0);
    } 

private:
    static bool jump(const std::vector<int>& nums, size_t index)
    {
        //std::cout<<"jumping to "<<index<<"\n";
        if(index >= nums.size()) return false;
        
        if(index == nums.size() - 1) return true;

        int can_jump = nums[index];
        if(can_jump == 0) return false;

        for(int i=1; i<=can_jump; i++)
        {
            if(jump(nums, index + i)) return true;
        }

        return false;
    }

    static int jump(const std::vector<int>& nums, size_t index, int count)
    {
        if(index >= nums.size())     return -1; 
        if(index == nums.size() - 1) return count;

        int can_jump = nums[index];
        if(can_jump == 0) return -1;

        std::vector<int> jumps;
        for(int i=1; i<=can_jump; i++)
        {
            int j = jump(nums, index + i, count + 1);
            if(j > 0) jumps.push_back(j);
        }

        return *std::min_element(jumps.begin(), jumps.end());
    }
};