#pragma once
#include "gtest/gtest.h"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>

class TestBit : public ::testing::Test
{
public:
    virtual void SetUp(void) { }
    virtual void TearDown(void) { }
    static void SetUpTestSuite() { }
    static void TearDownTestSuite() { }

    //showcases type conversion between double and uint64_t
    void dPrecision(const uint64_t n) 
    {
        const double nearest        = static_cast<double>(n);
        const uint64_t nearestAsInt = static_cast<uint64_t>(nearest);
        double ub, lb;
        if ((nearestAsInt == 0 && nearest > 1) // Overflow check
            || nearestAsInt >= n)
        {
            std::cout<<"Overflowed!\n";
            ub = nearest;
            lb = std::nextafter(nearest, nearest - 999999); // ? 
        } else {
            lb = nearest;
            ub = std::nextafter(nearest, std::numeric_limits<double>::infinity());
        }

        // If our int can be exactly represented as a double, set both bounds to that value
        if (std::round(ub) == std::round(lb))
        {
            ub = lb = std::round(ub);
        }

        std::cout << std::setprecision(50) 
            << "Original:          " << n << std::endl
            << "Nearest:           " << nearest << std::endl
            << "NearestAsInt:      " << nearestAsInt << std::endl
            << "Maybe lower bound: " << lb << std::endl
            << "Maybe upper bound: " << ub << std::endl
            << std::endl << std::endl;
    }
};
