#pragma once
#include "gtest/gtest.h"
#include <string>
#include <vector>
#include <set>
#include <iostream>

class TestStack : public ::testing::Test
{
public:
    virtual void SetUp(void) { }
    virtual void TearDown(void) { }
    static void SetUpTestSuite() { }
    static void TearDownTestSuite() { }

    bool areArrEqual(const std::vector<std::string>& arr1, const std::vector<std::string>& arr2)
    {
        std::multiset<std::string> s1, s2;
        for(auto str : arr1) { s1.insert(str); }
        for(auto str : arr2) { s2.insert(str); }

        if(s1 == s2) return true;
        else 
        {
            std::cout<<"Arr not equal - ";
            for(auto str : arr1) { std::cout<<str<<" "; } 
            std::cout<<" and ";
            for(auto str : arr2) { std::cout<<str<<" "; } 

            std::cout<<"\n";
            return false;
        }
    }
};
