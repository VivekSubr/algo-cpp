#pragma once
#include <climits>

int reverse_digits(int x)
{
    int rev(0);
    bool is_neg = (x < 0);

    x = std::abs(x);
    while(x > 0)
    {
        if (rev > INT_MAX / 10) return 0; //overflow when multiplying by 10

        // Get the last digit and add it to the reverse number
        rev = rev * 10 + x % 10;

        // Remove the last digit from the original number
        x /= 10;
    }

    return is_neg ? -rev : rev;
}