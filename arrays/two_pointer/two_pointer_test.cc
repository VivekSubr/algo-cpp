#include "two_pointer_test.h"
#include "palindrome.h"
#include "sum.h"
#include "sequence.h"
#include <unordered_map>
#include <tuple>

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
        {"abc", "ahbgdc", true},
        {"acb", "ahbgdc", false}
    };
    
    for(auto ele : tests)
    {
        ASSERT_EQ(is_sub_sequence(std::get<1>(ele), std::get<0>(ele)), std::get<2>(ele));
    }
}