#include "hyperLogLog.h"
#include "hyperLogLog_test.h"
#include <random>
#include <unordered_map>
#include <limits>
#include <tuple>

TEST_F(TestEstimate, simpleEstimate)
{ 
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 1000);

    for(int i=0; i<10; i++)
    {
        auto val = hashNormalize(dist(rng));
        ASSERT_TRUE(val < 1.0 && val > 0) << "hashNormalize, got " << val;
    }

    std::vector<uint> test = {10, 10, 50, 80, 100, 20, 50, 60, 80, 10}; //cardinality 7
    std::cout<<"Simple Estimate "<<simple_estimator(test)<<"\n";
}

TEST_F(TestEstimate, probabilisticEstimate)
{ 
    std::unordered_map<double, uint> test_leading_zero = {
        {0.0, 64}, //All zero
        {8.0, 60}, //8 in binary is 1000, hence 60 leading zeroes
        {1.0, 63} 
    };

    for(auto test : test_leading_zero)
    {
        ASSERT_EQ(count_leading_zeros(test.first), test.second) << "Failed for "<<test.first;
    }

    std::vector<uint> test = {10, 10, 50, 80, 100, 20, 50, 60, 80, 10}; //cardinality 7
    std::cout<<"Probabilistic Estimate, leading "<<probabilistic_estimator(test, probabalistic_estimate_type::leading_zeroes)<<"\n";
    std::cout<<"Probabilistic Estimate, trailing "<<probabilistic_estimator(test, probabalistic_estimate_type::trailing_zeroes)<<"\n";
}

TEST_F(TestEstimate, logLog)
{   
    std::vector<std::tuple<int, int, int>> zero_count = {
        {8.0, 56, 3}, //8 in binary is 1000, hence 60 leading zeroes ( - k=4, so 56) and 3 trailing.
    };

    uint k = 4;
    for(auto test : zero_count)
    {
        ASSERT_EQ(count_leading_zeros (std::get<0>(test), k), std::get<1>(test)) << "leading failed for  "<<std::get<0>(test);
        ASSERT_EQ(count_trailing_zeros(std::get<0>(test), k), std::get<2>(test)) << "trailing failed for "<<std::get<0>(test);
    }

    std::vector<uint64_t> get_bucket_index_test = {
        1152921504606846976, //000100000000...
        2305843009213693952, //001000000000...
        3458764513820540928,
        4611686018427387904
    };

    for(int i=1; i<5; i++) {
        ASSERT_EQ(get_bucket_index(i, k), get_bucket_index_test[i-1]) << "get_bucket_index failed for "<<i<<"\n";
        ASSERT_EQ(i, bucket_index(get_bucket_index(i, k), k)) << "bucket_index failed for "<<i<<", "<<get_bucket_index(i, k)<<"\n";
    }

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, UINT64_MAX);

    std::vector<uint64_t> test;
    for(int i=0; i<1000; i++) 
    {
        test.push_back(dist(rng));
    }
    ASSERT_EQ(test.size(), 1000);

    std::cout<<"cardinality "<<find_cardinality(test)<<"\n";
    std::cout<<"LogLog, leading "<<loglog(test, 4, probabalistic_estimate_type::leading_zeroes)<<"\n";
    std::cout<<"LogLog, trailing "<<loglog(test, 4, probabalistic_estimate_type::trailing_zeroes)<<"\n";
}

TEST_F(TestEstimate, hyperlogLog)
{   
    std::vector<std::tuple<uint64_t, uint64_t, uint64_t, uint64_t>> index_remain_test = {
        {1152921504606846977, 1, 1, 60}, //0001000...1
        {2305843009213693954, 2, 2, 60}
    };

    uint p = 4;
    for(auto test : index_remain_test)
    {
        auto in_remain = index_and_remainder(std::get<0>(test), p);
        ASSERT_EQ(in_remain.first,  std::get<1>(test));
        ASSERT_EQ(in_remain.second, std::get<2>(test));
        ASSERT_EQ(count_zeroes(in_remain.second, p), std::get<3>(test));
    }

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, UINT64_MAX);

    std::vector<uint64_t> test;
    for(int i=0; i<10000; i++) 
    {
        test.push_back(dist(rng));
    }

    std::cout<<"cardinality "<<find_cardinality(test)<<"\n";
    std::cout<<"HyperLogLog "<<hyperloglog(test, 4)<<"\n";
}

TEST_F(TestEstimate, hyperlogLog_plusplus)
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, UINT64_MAX);

    std::vector<uint64_t> test;
    for(int i=0; i<10000; i++) 
    {
        test.push_back(dist(rng));
    }

    std::cout<<"cardinality "<<find_cardinality(test)<<"\n";
    std::cout<<"HyperLogLog++ "<<hyperloglog_plusplus(test, 4)<<"\n";   
}

