#pragma once
#include "gtest/gtest.h"
#include <memory>
#include <vector>

class TestTree : public ::testing::Test
{
public:
    virtual void SetUp(void) { }
    virtual void TearDown(void) { }
    static void SetUpTestSuite() { }
    static void TearDownTestSuite() { }

    std::vector<std::shared_ptr<int>> arr2pts(const std::vector<int>& arr)
    {
        std::vector<std::shared_ptr<int>> ret;
        for(auto i : arr) {
            if(i == -1) ret.push_back(nullptr);
            else        ret.emplace_back(std::make_shared<int>(i));
        }
   
        return ret;
    }
};
