## Binary Search
```
int binary_search(const std::vector<int>& nums, int target)
{
    int start = 0, end = nums.size() - 1;
    while(start <= end)
    {
        int mid = start + (end - start)/2;
        if(nums.at(mid) == target) return mid;
        
        if(nums.at(mid) < target) start = mid + 1;
        else end = mid - 1;
    }

    return -1;
}
```

Binary search is a important algo to know. As can be seen, we remove half the problem space per iteration, hence the time complexity is O(log n)

When any problem has contraint that solution should be O(log n), it is likely binary search is part of the solution. 

## Max Sliding Window
Problem statement 
`"You are given an array of integers nums, there is a sliding window of size k which is moving from the very left of the array to the very right. You can only see the k numbers in the window. Each time the sliding window moves right by one position.

Return the max sliding window."`

First, we have the basic brute force method:
```
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
```

Note, this problem is not asking for sum, rather it asks for the max element in the sliding window.

To find the max sum of sub-sequence, brute force method is - try every possible sub-sequence. Complexity is O(n squared).

Now, let's use the standard library, 
```
vector<int> ret;
for (int i = 0; i + k <= (int)nums.size(); i++) {
    auto it = std::max_element(nums.begin() + i, nums.begin() + i + k);
    ret.push_back(*it);
}
```

std::max_element is O(n), hence complexity is O(n.k)... still overall O(n squared)

Now, consider the BST solution:
```
    std::vector<int> maxSlidingWindow(std::vector<int>& nums, int k) 
    {
        std::multiset<int> s;
        std::deque<std::multiset<int>::iterator> iters;   // one entry per element currently in the window
        std::vector<int> ret;

        for (int i = 0; i < k; i++) { iters.push_back(s.insert(nums[i])); } //load in the initial window of k elements
        ret.push_back(*s.rbegin());

        for (int i = k; i < (int)nums.size(); i++) 
        {
            s.erase(iters.front());   // erase the oldest element directly — no find of nums[i-k] 
            iters.pop_front();

            iters.push_back(s.insert(nums[i])); 
            ret.push_back(*s.rbegin()); //the largest element in the window
        }

        return ret;
    }
}
```

This uses multiset's ordering property, with the 'iters' deque preserving the actual array sequence. This is O(n). 


## Palindrome
Basically, check if [i to mid] == [j to mid], hence loop runs for n/2 iterations... hence O(n/2)... O(n).

*Palindrome Partition* is more of a dp problem.

## Sequence 
Consider sub_sequence function,

```
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
```

This is interesting, best case - the while loop finds substring immediatly and runs only for sub_str.size()... worst case it finds nothing and runs for str.size(), complexity is O(n).


Now, coming to **lengthOfLIS**


## Sum 
We have two_sum solved in two different ways, let's consider them.

First, we have a sliding window solution -

```
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
```

Worst case - target indices are in the middle, hence roughly O(n/2)... O(n)

However, the hashmap solution is more straigtforward and doesn't need the elements to be sorted.
```
std::pair<int, int> twoSum(std::vector<int>& nums, int target) 
{ 
    std::unordered_map<int, int> seen; // value -> index
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
```

The find is O(1), so this is again O(n) solution.
