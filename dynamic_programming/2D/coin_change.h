#pragma once
#include <vector>
/*
You are given an integer array coins representing coins of different denominations (e.g. 1 dollar, 5 dollars, etc) 
and an integer amount representing a target amount of money.

Return the number of distinct combinations that total up to amount. If it's impossible to make up the amount, return 0.

You may assume that you have an unlimited number of each coin and that each value in coins is unique.
*/

int change(int amount, const std::vector<int>& coins)
{
/*
eg: amount = 5, coins = [1,2,5]
    Combinations:

    5
    2 + 2 + 1
    2 + 1 + 1 + 1
    1 + 1 + 1 + 1 + 1

    Approach this as a state machine,
    S(i,a)for i∈[0..n], a∈[0..amount]

    Start state: S(0,0) 

    Transitions
        From S(i, a) you have two options:

        Use coin i (unlimited times)
        If a + coins[i] ≤ amount:
          S(i,a)→S(i,a + coins[i])


        Skip coin i (move to next denomination)
        S(i,a)→S(i + 1,a)

    End state: S(n, amount)

    Hence, build the dp table,
    dp[i][a] = number of ways to make amount a using first i coins
    (base case) dp[0][0] = 1 (1 way to make amount 0 with 0 coins)

    Transitions:
        For each coin i from 1 to n:
        For each amount a from 0 to amount:
            dp[i][a] = dp[i-1][a] (not using coin i)
            If a >= coins[i-1]:
                dp[i][a] += dp[i][a - coins[i-1]] (using coin i)
*/

    std::vector<std::vector<int>> dp(coins.size() + 1, std::vector<int>(amount + 1, 0));
    dp[0][0] = 1; // Base case

    for(int i = 1; i <= coins.size(); ++i)
    {
        for(int a = 0; a <= amount; ++a)
        {
            dp[i][a] = dp[i-1][a]; // Not using coin i

            if(a >= coins[i-1])
            {
                dp[i][a] += dp[i][a - coins[i-1]]; // Using coin i
            }
        }
    }

    return dp[coins.size()][amount];
}

