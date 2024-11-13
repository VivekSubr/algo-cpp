#pragma once
#include "gtest/gtest.h"
#include <algorithm>
#include <stdint.h>
#include <iostream>
#include <bitset>

class TestEstimate : public ::testing::Test
{
public:
    virtual void SetUp(void) { }
    virtual void TearDown(void) { }
    static void SetUpTestSuite() { }
    static void TearDownTestSuite() { }

    uint find_cardinality(std::vector<uint64_t> arr)
    {
        std::sort(arr.begin(), arr.end());
        auto newend = std::unique(arr.begin(), arr.end());
        return newend - arr.begin(); 
    }

    uint64_t reverse_bits(uint64_t num)
    {
        uint64_t reversedNumber = 0;
        int bitCount = sizeof(uint64_t) * 8; //size in bytes * 8 to get number of bits

        for (int i = 0; i < bitCount; i++) 
        {
            if (num & (1 << i)) { //Check if ith bit is set

                //Set the bit in the reversed location
                reversedNumber |= 1 << ((bitCount - 1) - i);
            }
        }

        return reversedNumber;
    }

    std::string strBits(uint64_t num)
    {
        std::bitset<64> temp(num);
        return temp.to_string();
    }

    //set the first k bits as this index
    uint64_t get_bucket_index(uint64_t index, uint k)
    {
        uint64_t ret = 0;
        uint     bitCount = sizeof(uint64_t) * 8; //size in bytes * 8 to get number of bits

        for(uint i = 0; i < k; i++) 
        {
            if(index & (1ULL << i))
            {
                ret |= (static_cast<uint64_t>(1) << (bitCount - k + i));
            }
        }

        return ret;
    }
};
