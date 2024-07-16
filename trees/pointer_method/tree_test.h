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
};
