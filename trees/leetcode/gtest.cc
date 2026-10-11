#include <gtest/gtest.h>

#include "top_k.h"

TEST(TopKTest, SupportsLargestAndMostFrequentQueries)
{
	{
		TopK top_k({3, 2, 1, 5, 6, 4});

		EXPECT_EQ(top_k.k_largest(2), 5);
		EXPECT_EQ(top_k.k_largest(2), 5);
	}

	{
		TopK top_k({3, 2, 1});

		top_k.add(4);

		EXPECT_EQ(top_k.k_largest(1), 4);
	}

	{
		TopK top_k({3, 3, 3});

		EXPECT_EQ(top_k.k_frequent(1), 3);

		top_k.add(3);

		EXPECT_EQ(top_k.k_frequent(1), 4);
	}
}
