A collection of example problems.

# Sub-Sets
*"Given an array nums of unique integers, return all possible subsets of nums.*

*The solution set must not contain duplicate subsets. You may return the solution in any order."*

So, what is the **choice** here? Each element can be in a sub-set or now, hence we have 2 power n total possible subsets. 

If choice results in a subset that's already chosen - backtrack.

Consider this implementation - 
```
    void choose_element(size_t index, const vector<int>& nums, 
            vector<int> cur_set, vector<vector<int>>& ret)
    {
        cur_set.push_back(nums.at(index));
        ret.push_back(cur_set);                 // every path is a valid subset

        for (size_t i = index + 1; i < nums.size(); i++)
            choose_element(i, nums, cur_set, ret);
    }

    vector<vector<int>> subsets(vector<int>& nums) 
    {
        vector<vector<int>> ret;
        ret.push_back({});

        for(size_t i=0; i<nums.size(); i++)
        {
            choose_element(i, nums, {}, ret);
        }

        return ret;
    }
```

More efficient to send cur_set by reference,
```
    void backtrack(size_t start, const vector<int>& nums,
               vector<int>& cur, vector<vector<int>>& ret)
    {
        ret.push_back(cur);
        for (size_t i = start; i < nums.size(); i++)
        {
            cur.push_back(nums[i]);
            backtrack(i + 1, nums, cur, ret);
            cur.pop_back(); //Create subset without this element, since every 
                            //sub-set  should be unique
        }
    }

    vector<vector<int>> subsets(vector<int>& nums)
    {
        vector<vector<int>> ret;
        vector<int> cur;
        backtrack(0, nums, cur, ret);
        return ret;
    }
```

Iterative version,
```
    vector<vector<int>> subsets(vector<int>& nums)
    {
        vector<vector<int>> ret{{}};              // start with the empty set

        for (int x : nums)
        {
            size_t n = ret.size();                // snapshot: only extend existing subsets
            for (size_t i = 0; i < n; i++)
            {
                vector<int> s = ret[i];
                s.push_back(x);
                ret.push_back(std::move(s));
            }
        }
    
        return ret;
    }
```

Let's work out the thought process that lead to this code

**What is the choice?**
Choose element from nums which is not in current set.
If set is not in in subsets, then add.

**How should worker function look?**
We have variables -
* ret, vector<vector<int>>& - array of sub-sets
* working set, vector<int>& - working set
* input nums, vector<int>&

if working set is not in ret:
    add working set to set

Add element from nums to working set.

# Combination Sum
*"You are given an array of distinct integers nums and a target integer target. Your task is to return a list of all unique combinations of nums where the chosen numbers sum to target.*

*The same number may be chosen from nums an unlimited number of times. Two combinations are the same if the frequency of each of the chosen numbers is the same, otherwise they are different.*

*You may return the combinations in any order and the order of the numbers in each combination can be in any order."*

**What is the choice?**
A number from num that leads to sum = target.

**How should worker function look?**
We have variables - 
* ret - vector<vector<int>>&, array of ways to sum to target.
* int target 
* Input nums
* Working set, vector<int>&

Choose num from nums that'll bring sum closer to target. Abort if more than target... so the **choice** is
    if element + working set == target : 
        add to working set, add set to ret

    if element + working set < target:
        add element to working set
        continue recursion

    if element + working set > target:
        continue recursion

So, let's get the main function
```
vector<vector<int>> combination_sum(vector<int>& nums, int target)
{
    vector<vector<int>> ret;
    choose_ele(0, target, nums, {}, ret);
    return ret;
}
```

And the worker function being
``` void choose_ele(size_t index, int cur_target, const vector<int>& nums, vector<int>& cur_set, vector<vector<int>& ret)```

As suggested by the signature, idea is - each worker func will choose elements that will bring it's running target closer to target.
```
void choose_ele(size_t index, int cur_target, const vector<int>& nums, vector<int>& cur_set, vector<vector<int>& ret)
{
    for (size_t i = index; i < nums.size(); i++)
    {
        int nTarget = cur_target - nums.at(i);
        if (nTarget < 0) continue;      // too big, skip (use break if nums is sorted)

        cur_set.push_back(nums.at(i));
        if (nTarget == 0)
            ret.push_back(cur_set);     // hit; no need to recurse, all values are positive
        else
            choose_ele(i, nTarget, nums, cur_set, ret);   // i, not i+1: reuse allowed
        
        cur_set.pop_back();             // single un-choose for every path
    }
}

```