#pragma once 
#include <vector>
#include <set>
#include <algorithm>

//https://leetcode.com/problems/combination-sum/description/


class CombinationSum 
{
public:
    static std::vector<std::vector<int>> findSum(std::vector<int>& candidates, int target)
    {
        std::vector<std::vector<int>> ret;
        std::vector<int> combination;
        std::cout<<"Trying to find combination sum "<<target<<" candidates size "<<candidates.size()<<"\n";
        
        do_findSum(candidates, combination, ret, 0, 0, target);
        return ret;
    }

    static std::vector<std::vector<int>> findSumUnique(std::vector<int>& candidates, int target)
    {
        std::vector<std::vector<int>> ret;
        std::set<int> combination;
        std::cout<<"Trying to find combination sum "<<target<<" candidates size "<<candidates.size()<<"\n";
        
        do_findSumUnique(candidates, combination, ret, 0, 0, target);
        return ret;
    }

private:
    static bool do_findSum(const std::vector<int>& candidates, std::vector<int>& combination, 
                            std::vector<std::vector<int>>& ret, 
                            int cur_index, int sum, int target)
    {
        std::cout<<"do_findSum "<<cur_index<<" "<<sum<<" "<<target<<"\n";
        if(sum > target) {
            std::cout<<"Failed to find "<<cur_index<<"\n";
            return false;
        }

        if(sum == target) 
        {
            std::cout<<"Found! { ";
            for(auto it : combination) std::cout<<it<<" ";
            std::cout<<"}\n";

            ret.emplace_back(combination);
            return true;
        }

        for(size_t i=cur_index; i<candidates.size(); i++)
        {
            combination.push_back(candidates[i]);
            sum += candidates[i];

            do_findSum(candidates, combination, ret, i, sum, target);

            //backtrack!
            combination.pop_back(); 
            sum-=candidates[i];
        }

        return false;
    }

    static bool do_findSumUnique(const std::vector<int>& candidates, std::set<int>& combination, 
                            std::vector<std::vector<int>>& ret, 
                            int cur_index, int sum, int target)
    {
        std::cout<<"do_findSumUnique "<<cur_index<<" "<<sum<<" "<<target<<"\n";
        if(sum > target) {
            std::cout<<"Failed to find "<<cur_index<<"\n";
            return false;
        }

        if(sum == target) 
        {
            std::vector<int> temp;
            std::cout<<"Found! { ";
            for(auto it : combination) { 
                temp.push_back(it);
                std::cout<<it<<" ";
            }
            std::cout<<"}\n";

            ret.emplace_back(temp);
            return true;
        }

        for(size_t i=cur_index; i<candidates.size(); i++)
        {
            combination.insert(candidates[i]);
            sum += candidates[i];

            do_findSumUnique(candidates, combination, ret, i+1, sum, target);

            //backtrack!
            combination.erase(candidates[i]); 
            sum-=candidates[i];
        }

        return false;
    }
};
