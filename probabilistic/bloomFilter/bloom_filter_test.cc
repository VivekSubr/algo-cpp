#include "bloom_filter_test.h"
#include "bloom_filter.h"

TEST_F(BloomFilterTest, TestBloomFilter)
{
    bloom_filter<int> bf([](const int& ele) { return std::hash<int>{}(ele); }, 128);
    bf.add(10);
    bf.add(20);
    bf.add(30);

    EXPECT_TRUE(bf.exists(10));
    EXPECT_TRUE(bf.exists(20));
    EXPECT_TRUE(bf.exists(30));
    EXPECT_FALSE(bf.exists(40));

    bf.remove(20);
    EXPECT_FALSE(bf.exists(20));
}