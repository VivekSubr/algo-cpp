#pragma once
#include <vector>
#include <limits>
#include <algorithm>
#include <string>
#include <unordered_map>

int maxSubArraySum(const std::vector<int>& arr)
{ //Kadane's algo
    int best_sum = std::numeric_limits<int>::min();
    int cur_sum  = 0;

    for(int x : arr)
    {
        cur_sum  = std::max(x, cur_sum + x);
        best_sum = std::max(best_sum, cur_sum);
    }

    return best_sum;
}

//return the minimal length of a subarray whose sum is greater than or equal to target
//Sliding window
std::pair<std::size_t, std::size_t> minSubArrayWithTarget(const std::vector<int>& arr, int target)
{
    std::size_t start(0), end(arr.size() - 1);
    int         window_sum(0), try_sum(0);

    for(std::size_t i=0; i<arr.size(); i++)
    {
        window_sum = window_sum + arr[i];
        int try_end(end), try_start(start);
        if(window_sum >= target)
        {
            try_end = i;

            int nWindow = window_sum;
            try_sum = window_sum;
            while(try_sum >= target && start < end) 
            {
                try_sum = try_sum - arr[try_start];              
                if(try_sum >= target) 
                {
                    try_start++;
                    nWindow = try_sum;
                }
            }

            window_sum = nWindow;
            if(try_end - try_start < end - start)
            {
                end   = try_end;
                start = try_start;
            }
        }
    }

    return std::pair<std::size_t, std::size_t>(start, end);
}

//https://leetcode.com/problems/minimum-window-substring/
std::string minWindowSubstring(const std::string& str, const std::string& subStr)
{
    if(subStr.size() > str.size()) return "";

    std::unordered_map<char, int> subStrMap;
    for(auto ch : subStr)
    {
        if(subStrMap.find(ch) != subStrMap.end()) subStrMap[ch] = subStrMap[ch] + 1;  
        else                                      subStrMap[ch] = 1;
    }

    int start(0), end(-1);
    std::unordered_map<char, int> windowMap;
    for(int i=0; i<str.size(); i++)
    {
        if(windowMap.find(str[i]) != windowMap.end()) windowMap[str[i]] = windowMap[str[i]] + 1;  
        else                                          windowMap[str[i]] = 1;

        //if the string map is subset of window map
        if(std::all_of(subStrMap.begin(), subStrMap.end(), 
                [=](const auto& ele) 
                { 
                    if(windowMap.find(ele.first) == windowMap.end()) return false;
                    return windowMap.at(ele.first) >= ele.second;
                }))
        {
            end = i;
            auto nWindow = windowMap;
            while(start < end)
            { 
                nWindow.at(str[start]) = nWindow.at(str[start]) - 1;
                if(nWindow.at(str[start]) <= 0) nWindow.erase(str[start]);

                if(subStrMap.find(str[start]) != subStrMap.end())
                {
                    if(nWindow.find(str[start]) == nWindow.end())         break;
                    if(subStrMap.at(str[start]) < nWindow.at(str[start])) break;
                }

                ++start;
                windowMap = nWindow;
            }
        }
    }

    return std::string(str.begin() + start, str.begin() + end + 1);
}