#include "bit_manip_test.h"
#include "utils.h"
#include <unordered_map>
#include <algorithm>

TEST_F(TestBit, ReverseBits)
{
    std::unordered_map<std::string, uint32_t> tests = {
        {"00000010100101000001111010011100", 964176192},
        {"11111111111111111111111111111101", 3221225471}
    };

    for(auto test: tests)
    {
        std::string revBits(test.first.rbegin(), test.first.rend());
        ASSERT_EQ(strBitsToNum(revBits), test.second) << "str bits to num "<< revBits;
        ASSERT_EQ(reverse_bits(strBitsToNum(test.first)), test.second) << "Failed to reverse " << test.first;
    }
}

TEST_F(TestBit, CountZeroes)
{
    std::unordered_map<std::string, uint32_t> tests = {
        {"10000000000000000000000000000", 28}
    };

    for(auto test: tests)
    {
        ASSERT_EQ(count_zeroes(test.first), test.second);
    }
}

TEST_F(TestBit, DoublePrecision)
{
    dPrecision(std::numeric_limits<uint64_t>::max());         // Rounds up
    dPrecision(std::numeric_limits<uint64_t>::max() - 2046);  // Rounds down
    dPrecision(9999999999999999);
    dPrecision(10);

    dPrecision(1152921504606846976);
}