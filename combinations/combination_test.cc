#include "combinations_test.h"
#include "combinations.h"
#include "combinations_sum.h"

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

TEST_F(TestCombinations, CombinationSum)
{
    std::vector<int> problem{2,3,6,7};
    auto sol = CombinationSum::findSum(problem, 7);
    ASSERT_EQ(sol.size(), 2);

    std::vector<int> problem2{2,3,5};
    sol = CombinationSum::findSum(problem2, 8);
    ASSERT_EQ(sol.size(), 3);

    std::vector<int> problem3{10,1,2,7,6,1,5};
    sol = CombinationSum::findSumUnique(problem3, 8);
    ASSERT_EQ(sol.size(), 4);
}