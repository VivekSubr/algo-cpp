#include "linked_list_test.h"
#include "linked_list.h"
#include "merge_k_sorted.h"

TEST_F(TestList, Basic)
{
}

TEST_F(TestList, Merge2Lists)
{
    auto merged1 = merge_2_sorted(createList({1, 2, 4}), createList({1, 3, 4}));
    ASSERT_EQ(length(merged1), 6);

    ASSERT_TRUE(isEqual(merged1, createList({1, 1, 2, 3, 4, 4})));
}