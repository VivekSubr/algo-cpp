#include "merge_sort_test.h"
#include "merge_sort.h"
#include <gmock/gmock-matchers.h>
using namespace testing;

TEST_F(MergeSortTest, MergeSort)
{
    std::vector<int> tArr1 = {4,2,1,6,7};
    merge_sort(tArr1);
    ASSERT_THAT(tArr1, ElementsAre(1,2,4,6,7));
}

TEST_F(MergeSortTest, MergeIntervals)
{
    std::list<std::pair<int, int>> tInt1 = {{1,3},{2,6},{8,10},{15,18}};
    merge_intervals(tInt1);
    ASSERT_THAT(tInt1, ElementsAre(std::list<std::pair<int, int>>{{1,6},{8,10},{15,18}}));

    std::list<std::pair<int, int>> tInt2 = {{1,4},{4,5}};
    merge_intervals(tInt2);
    ASSERT_THAT(tInt1, ElementsAre(std::list<std::pair<int, int>>{{1,5}}));
}