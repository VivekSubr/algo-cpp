#pragma once
#include "gtest/gtest.h"
#include <vector>
#include <string>
#include <iostream>

class ArrayTest : public ::testing::Test
{
public:
    virtual void SetUp(void) { }
    virtual void TearDown(void) { }
    static void SetUpTestSuite() { }
    static void TearDownTestSuite() { }

    void printVector(const std::vector<std::string>& opArr)
    {
        for(auto op : opArr)
        {
            std::cout<<op<<"\n";
        }
    }
};
