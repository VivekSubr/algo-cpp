#pragma once
#include <vector>
#include <limits>
#include <algorithm>

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