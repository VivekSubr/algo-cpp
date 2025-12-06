#include "stack_test.h"
#include "reverse_polish.h"
#include "generate_paranthesis.h"
#include "misc.h"

TEST_F(TestStack, Inflix2PostFlix)
{
    std::vector<std::pair<std::string, std::string>> tests = {
        {"a+b*c+d", "abc*+d+"},
        {"A*(B+C)/D", "ABC+*D/"}
    };

    for(auto test : tests)
    {
        ASSERT_EQ(inflix2postflix(test.first), test.second);
    }
}

TEST_F(TestStack, Eval)
{
}

TEST_F(TestStack, GenerateParanthesis)
{
    std::unordered_map<int, std::vector<std::string>> tests = {
        {1, {"()"} },
        {3, {"((()))","(()())","(())()","()(())","()()()"} }
    };

    for(auto test : tests)
    {
        ASSERT_TRUE(areArrEqual(generate_parantheses(test.first), test.second));
    }
}

TEST_F(TestStack, DailyTemperatures)
{
    std::vector<std::pair<std::vector<int>, std::vector<int>>> tests = {
        {{73, 74, 75, 71, 69, 72, 76, 73}, {1, 1, 4, 2, 1, 1, 0, 0}},
        {{30, 40, 50, 60}, {1, 1, 1, 0}},
        {{30, 60, 90}, {1, 1, 0}}
    };

    for(auto test : tests)
    {
        ASSERT_EQ(dailyTemperatures(test.first), test.second);
    }
}