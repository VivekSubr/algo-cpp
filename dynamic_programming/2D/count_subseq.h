#pragma once
#include <string>
#include <vector>
#include "utils.h"

//neetcode.io/problems/count-subsequences?list=neetcode250
/*
	You are given two strings s and t, both consisting of english letters.
	Return the number of distinct subsequences of s which are equal to t.
*/
int count_subseq(std::string s, std::string t)
{
/*
	Dynamic Programming works by solving smaller subproblems and combining their results.
	When a problem involves two sequences or two dimensions (like two strings, a grid, or two parameters), we often use a 2D DP table because:
		Each axis represents one dimension of the problem.
		Each cell represents the solution for a subproblem defined by prefixes or partial states

	We build a DP where dp[i][j] = number of ways the first i characters of s can form the first j characters of t.
	
	Transitions
		If t[j-1] != s[i-1]
			We cannot match them, so we skip s[i-1]:
				dp[i][j] = dp[i-1][j]
		
		If t[j-1] == s[i-1]
			We can either use s[i-1] to match t[j-1] or skip it:
				dp[i][j] = dp[i-1][j] + dp[i-1][j-1]

	Base cases

	dp[i][0] = 1 for all i (empty t can be formed by deleting all characters of s in exactly one way).
	dp[0][j] = 0 for all j > 0 (non-empty t cannot be formed from empty s).

	Answer is dp[|s|][|t|].
*/

	std::vector<std::vector<int>> dp(s.size() + 1);
	/*
		Initial matrix is
            c   a   t
	c	1   0   0   0
	a	1   0   0   0
	a	1   0   0   0
    a	1   0   0   0
	t	1   0   0   0
	*/

	for(size_t i=0; i<s.size()+1; i++)
	{
		dp[i].resize(t.size()+1, 0);
		dp[i][0] = 1;
	}

	std::cout<<"Initial DP Matrix:\n";
	print_matrix(dp);
	
	for(size_t i=1; i<=s.size(); i++)
	{
		for(size_t j=1; j<=t.size(); j++)
		{
			if(t.at(j-1) != s.at(i-1))
			{
				//we are reseting here
				dp.at(i).at(j) = dp.at(i-1).at(j);
			}
			else 
			{
				//basically, each addition means more matching
				dp.at(i).at(j) = dp.at(i-1).at(j) + dp.at(i-1).at(j-1);
			}
		}
	}

	std::cout<<"Final DP Matrix:\n";
	print_matrix(dp);
	
	return dp.at(s.size()).at(t.size());
}

/*
	Working this out,
       c   a   t
       0   0   0
c	1  1   1   1          //s[0] = t[0] = 'c' --> dp[1][1] = dp[0][1] + dp[0][0] = 0 + 1 = 1, but then no matches of 'c' with 'a' or 't'
a	1  1   2   2          //s[1] = t[1] = 'a' --> dp[2][2] = dp[1][2] + dp[1][1] = 1 + 1 = 2, but no match of 'a' with 't'
t	1  1   1   3          //s[2] = t[2] = 't' --> dp[3][3] = dp[2][3] + dp[2][2] = 1 + 2 = 3
a	1  1   2   2          
a   1  1   2   2
t   1  1   1   3

answer is dp[5][3] = 3

*/