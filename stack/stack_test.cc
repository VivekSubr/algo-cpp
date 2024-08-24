#include "stack_test.h"
#include "reverse_polish.h"

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