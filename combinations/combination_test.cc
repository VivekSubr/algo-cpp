#include "combinations_test.h"
#include "combinations.h"

TEST_F(TestCombinations, GetCombinations)
{
    std::vector<int> test{1, 2, 3};

    Combination<int> C(test);
    auto comb_arr = C.getCombinations(2);

    for(auto comb : comb_arr)
    {
        for(auto ele : comb)
        {
            std::cout<<ele<<" ";
        } std::cout<<"\n";
    }
}