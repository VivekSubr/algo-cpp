#pragma once

/*
There is an m x n grid where you are allowed to move either down or to the right at any point in time.

Given the two integers m and n, return the number of possible unique paths that can be taken from the top-left corner 
of the grid (grid[0][0]) to the bottom-right corner (grid[m - 1][n - 1]).

You may assume the output will fit in a 32-bit integer.
*/

int uniquePaths(int m, int n) 
{
// Let dp[i][j] be the number of ways to reach cell (i, j).
// now, since we can only move right or down, the number of ways to reach (i, j) is the sum of the number of ways to reach
// the cell directly above it (i-1, j) and the cell directly to the left of it (i, j-1).
// Therefore, the recurrence relation is:
// dp[i][j] = dp[i-1][j] + dp[i][j-1]

    std::vector<std::vector<int>> dp(m, std::vector<int>(n, 0));

    // Base case: There is only one way to reach any cell in the first row or first column
    for(int i = 0; i < m; ++i) dp[i][0] = 1;
    for(int j = 0; j < n; ++j) dp[0][j] = 1;

    // Fill the dp table
    for(int i = 1; i < m; ++i) {
        for(int j = 1; j < n; ++j) {
            dp[i][j] = dp[i-1][j] + dp[i][j-1];
        }
    }

    return dp[m-1][n-1];
}

/*
You are given an m x n integer array grid. There is a robot initially located at the top-left corner (i.e., grid[0][0]). 
The robot tries to move to the bottom-right corner (i.e., grid[m - 1][n - 1]). The robot can only move either down or right
at any point in time.

An obstacle and space are marked as 1 or 0 respectively in grid. A path that the robot takes cannot include any square that is 
an obstacle.

Return the number of possible unique paths that the robot can take to reach the bottom-right corner.

The testcases are generated so that the answer will be less than or equal to 2 * (10^9).
*/

int uniquePaths_withObstacle(int m, int n) 
{
}