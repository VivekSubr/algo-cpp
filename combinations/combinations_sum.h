#pragma once 
#include <vector>
#include <algorithm>
#include <iostream>

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

    //Each solution must be unique, ie can't have solutions which are just other combination of exisiting solution.
    static std::vector<std::vector<int>> findSumUnique(std::vector<int>& candidates, int target)
    {
        std::vector<std::vector<int>> ret;
        std::vector<int> combination;
        std::cout<<"Trying to find combination sum "<<target<<" candidates size "<<candidates.size()<<"\n";
        
        do_findSum(candidates, combination, ret, 0, 0, target);
        
        filter_non_unique(ret);
        return ret;
    }

private:
    static bool do_findSum(const std::vector<int>& candidates, std::vector<int>& combination, 
                            std::vector<std::vector<int>>& ret, 
                            int cur_index, int sum, int target)
    {
        //std::cout<<"do_findSum "<<cur_index<<" "<<sum<<" "<<target<<"\n";
        if(sum > target) {
            //std::cout<<"Failed to find "<<cur_index<<"\n";
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

    static void filter_non_unique(std::vector<std::vector<int>>& combinations)
    { //Here, filter out elements which are simply combinations of each other.
      
      for(int i=0; i<combinations.size(); i++)
      {
        for(int j=0; j<combinations.size(); j++)
        {
            if(i == j) continue;

            if(combinations[i].size() == combinations[j].size())
            {

            }

        }
      }
    }
};
