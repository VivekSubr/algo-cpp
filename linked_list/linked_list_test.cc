#include "linked_list_test.h"
#include "linked_list.h"
#include "merge_k_sorted.h"
#include "in_place_op.h"

TEST_F(TestList, Basic)
{
    auto list = createList({1, 2, 3, 4, 5});
    reverse(list);

    ASSERT_TRUE(isEqual(list, createList({5, 4, 3, 2, 1}))) << "Expected: 5 4 3 2 1, Got: " << print_list(list);
}

TEST_F(TestList, Merge2Lists)
{
    auto merged1 = merge_2_sorted(createList({1, 2, 4}), createList({1, 3, 4}));
    ASSERT_EQ(length(merged1), 6);

    ASSERT_TRUE(isEqual(merged1, createList({1, 1, 2, 3, 4, 4})));
}

/*
make visualize FILE=in_place_op.h FUNCTION=reverse_between ALGOVIZ_ARGS='--test TestList.ReverseBetween --timeout 1'

To run only this TC: ./build/listTest --gtest_filter=TestList.ReverseBetween
*/
TEST_F(TestList, ReverseBetween)
{
    auto list = createList({1, 2, 3, 4, 5});
    reverse_between(list, 2, 4);

    ASSERT_TRUE(isEqual(list, createList({1, 4, 3, 2, 5}))) << "Expected: 1 4 3 2 5, Got: " << print_list(list);
}