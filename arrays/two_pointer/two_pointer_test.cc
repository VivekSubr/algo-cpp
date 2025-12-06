#include "two_pointer_test.h"
#include "palindrome.h"
#include "sum.h"
#include "sequence.h"
#include "max_sliding_window.h"
#include "binary_search.h"
#include <unordered_map>
#include <tuple>
#include <numeric>

TEST_F(TwoPointerTest, BinarySearch)
{
    std::vector<std::tuple<std::vector<int>, int, int>> tests = {
        {{-1,0,3,5,9,12}, 9, 4},
        {{1,0,3,5,9,12}, 2, -1},
    };

    for(auto ele : tests)
    {
        ASSERT_EQ(binary_search(std::get<0>(ele), std::get<1>(ele)), std::get<2>(ele))
            << "Failed for input " << 
            std::accumulate(std::get<0>(ele).begin(), std::get<0>(ele).end(), std::string(""), 
            [](const std::string& a, int b) { return a + std::to_string(b) + " "; }) 
            << " and target " << std::get<1>(ele);
    }
}

TEST_F(TwoPointerTest, SearchInRotatedSortedArray)
{
    std::vector<std::tuple<std::vector<int>, int, int>> tests = {
        {{4,5,6,7,0,1,2}, 0, 4},
        {{4,5,6,7,0,1,2}, 3, -1},
        {{1}, 0, -1}
    };

    for(auto ele : tests)
    {
        ASSERT_EQ(searchRotatedArray(std::get<0>(ele), std::get<1>(ele)), std::get<2>(ele))
            << "Failed for input " << 
            std::accumulate(std::get<0>(ele).begin(), std::get<0>(ele).end(), std::string(""), 
            [](const std::string& a, int b) { return a + std::to_string(b) + " "; });
    }
}

TEST_F(TwoPointerTest, ValidPalindrome)
{
    std::unordered_map<std::string, bool> tests = {
        {"A man, a plan, a canal: Panama", true},
        {"race a car", false},
        {"0P", false},
    };

    for(auto ele : tests)
    {
        if(ele.second) ASSERT_TRUE (is_valid_palindrome(ele.first));
        else           ASSERT_FALSE(is_valid_palindrome(ele.first));
    }
}

TEST_F(TwoPointerTest, SubSequence)
{
    std::vector<std::tuple<std::string, std::string, bool>> tests = {
        {"abc", "ahbgdabc", true},
        {"acb", "ahbgdc",   false}
    };
    
    for(auto ele : tests)
    {
        ASSERT_EQ(is_sub_sequence(std::get<1>(ele), std::get<0>(ele)), std::get<2>(ele))
            << "Failed for " << std::get<0>(ele) << ", " << std::get<1>(ele);
    }
}

TEST_F(TwoPointerTest, ThreeSum)
{
    std::vector<std::tuple<std::vector<int>, std::set<std::tuple<int, int, int>>>> tests = {
        {{-1,0,1,2,-1,-4}, {{std::make_tuple(-1,-1,2), std::make_tuple(-1,0,1)}}},
        {{0,1,1}, {}},
        {{0,0,0}, {{std::make_tuple(0,0,0)}}}
    };

    for(auto ele : tests)
    {
        ASSERT_EQ(three_sum(std::get<0>(ele)), std::get<1>(ele));
    }
}

TEST_F(TwoPointerTest, LengthOfLIS)
{
    std::vector<std::tuple<std::vector<int>, int>> tests = {
        {{10,9,2,5,3,7,101,18}, 4},
        {{0,1,0,3,2,3}, 4},
        {{7,7,7,7,7,7,7}, 1}
    };

    for(auto ele : tests)
    {
        ASSERT_EQ(lengthOfLIS(std::get<0>(ele)), std::get<1>(ele)) << "Failed for input " << 
            std::accumulate(std::get<0>(ele).begin(), std::get<0>(ele).end(), std::string(""), 
            [](const std::string& a, int b) { return a + std::to_string(b) + " "; });
    }
}

TEST_F(TwoPointerTest, MaxSlidingWindow)
{
    std::vector<int> nums = {1,3,-1,-3,5,3,6,7};
    int k = 3;
    std::vector<int> expected = {3,3,5,5,6,7};

    ASSERT_EQ(maxSlidingWindow(nums, k), expected);
}
