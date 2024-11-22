#pragma once
#include <stdint.h>
#include <cmath>
#include <bitset>
#include <string>

uint32_t reverse_bits(uint32_t num)
{
    uint32_t reversedNumber = 0;
    int bitCount = sizeof(uint32_t) * 8; //size in bytes * 8 to get number of bits

    for (int i = 0; i < bitCount; ++i) {
        if (num & (1 << i)) { //Check if ith bit is set

            //Set the bit in the reversed location
            reversedNumber |= 1 << ((bitCount - 1) - i);
        }
    }

    return reversedNumber;
}

std::string strBits(uint32_t num)
{
    std::bitset<32> temp(num);
    return temp.to_string();
}

uint32_t strBitsToNum(const std::string& bits)
{
    uint32_t ret(0), count(0);
    for(auto rit = bits.rbegin(); rit != bits.rend(); rit++)
    {
        if     (*rit == '1') ret += std::pow(2, count);
        else if(*rit != '0')
        {
            throw std::runtime_error("not valid binary"); 
        }

        ++count;
    }

    return ret;
}

int count_zeroes(const std::string& str)
{
    int ret = 0;
    for(auto ch : str)
    {
        if(ch == '0') ++ret;
    }

    return ret;
}

uint hexToDigit(char ascii)
{
    uint hexDigit(ascii);
    if     (ascii >='0' && ascii <='9') hexDigit = ascii - '0';
    else if(ascii >='a' && ascii <='f') hexDigit = ascii -'a' + 10;
    else if(ascii >='A' && ascii <='F') hexDigit = ascii -'A' + 10;

    return hexDigit;
}