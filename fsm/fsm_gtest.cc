#include "fsm_gtest.h"
#include "simple_regex_fsm.h"
#include <tuple>
#include <vector>

TEST_F(TestFsm, TestSimpleRegexFsm)
{
    std::vector<std::tuple<std::string, std::string, bool>> test_arr =
    {
        {"aa", "a",  false}, 
        {"ab", ".*", true},
        {"aa", "a*", true},
        {"aab", "c*a*b", true}
    };

    for(std::size_t i=0; i<test_arr.size(); i++)
    {
        Regex r(std::get<1>(test_arr[i]));
        ASSERT_EQ(r.match(std::get<0>(test_arr[i])), std::get<2>(test_arr[i])) << "failed at index " << i;
    }

    std::cout<<"TestSimpleRegexFsm, "<<test_arr.size()<<" cases passed\n";
}