#pragma once

#include "gtest/gtest.h"
#include <vector>
#include <string>
#include <iostream>

class TestList : public ::testing::Test
{
public:
    virtual void SetUp(void) { }
    virtual void TearDown(void) { }
    static void SetUpTestSuite() { }
    static void TearDownTestSuite() { }
};
