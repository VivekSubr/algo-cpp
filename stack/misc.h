#pragma once
#include <vector>
#include <stack>

//leetcode.com/problems/daily-temperatures/
std::vector<int> dailyTemperatures(std::vector<int>& temperatures) 
{
    std::stack<std::size_t> s; //store indices
    std::vector<int> ret(temperatures.size(), 0); //'0' meaning no warmer day ahead

    for(std::size_t i = 0; i < temperatures.size(); i++)
    {
        while(!s.empty() && temperatures[i] > temperatures[s.top()])
        { //pop the stack till we find an index where temp is greater
            auto idx = s.top();
            s.pop();
            ret[idx] = i - idx; //difference in indices is the number of days to wait
        }

        s.push(i); //pushing each index into stack
    }

    return ret;
}

//leetcode.com/problems/largest-rectangle-in-histogram/description/
//Given an array of integers heights representing the histogram's bar height 
//where the width of each bar is 1, return the area of the largest rectangle in the histogram.
int largestRectangleArea(std::vector<int>& heights) 
{
  //So, the rectangle can be formed by any four sides, basically any two bars.       
}