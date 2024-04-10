#pragma once
#include <string>
#include <vector>
#include <algorithm>

//https://en.wikipedia.org/wiki/Edit_distance
//https://en.wikipedia.org/wiki/Levenshtein_distance

std::string tail(const std::string& str)
{
    return std::string(str.begin(), str.end() - 1);
}

char head(const std::string& str)
{
    return str[0];
}

int levenshtein_distance(const std::string& strA, const std::string& strB)
{
    if(strA.empty() || strB.empty()) return 0;

    if(head(strA) == head(strB)) {
        return levenshtein_distance(tail(strA), tail(strB));
    }

    return 1 + std::min({levenshtein_distance(tail(strA), strB),
                         levenshtein_distance(strA, tail(strB)),
                         levenshtein_distance(tail(strA), tail(strB))});
}

//This is a dp based solution for https://leetcode.com/problems/edit-distance/
std::pair<int, std::vector<std::string>> edit_distance(const std::string& strA, const std::string& strB)
{
    if(strA.empty() || strB.empty()) return { 0, {}};

    //The idea is to build a 2D matrix dp where dp[i][j] represents the minimum number of operations required to 
    //transform the substring word1[0...i-1] into the substring word2[0...j-1].

    /*
    If word1[i-1] == word2[j-1], then dp[i][j] = dp[i-1][j-1]. 
    That is, no operation is required because the characters at positions i-1 and j-1 are already the same.

    Otherwise, dp[i][j] is the minimum of the following three values:
        dp[i-1][j-1] + 1: replace the character at position i-1 in word1 with the character at position j-1 in word2.
        dp[i-1][j] + 1: delete the character at position i-1 in word1.
        dp[i][j-1] + 1: insert the character at position j-1 in word2 into word1 at position i.
    */
    std::vector<std::vector<int>> dp(strA.size() + 1, std::vector<int>(strB.size() + 1, 0));

    //Thing of the dp matrix as all 0s as two empty strings
    for(std::size_t i = 1; i <= strA.size(); i++) dp[i][0] = i; //Moving forward in i, adding each character 
    for(std::size_t j = 1; j <= strB.size(); j++) dp[0][j] = j; //Moving forward in j, adding each character

    for(std::size_t i = 1; i <= strA.size(); i++)
    {
      for(std::size_t j = 1; j <= strB.size(); j++)
      {
        if(strA.at(i - 1) == strB.at(j - 1)) {
            dp[i][j] = dp[i - 1][j - 1]; //no operator, move forward
        }
        else {
            dp[i][j] = std::min({dp[i - 1][j - 1], dp[i - 1][j], dp[i][j - 1]}) + 1;
        } 
      }
    }

    std::vector<std::string> opArr;
    
    //Traverse the dp matrix now and list out all operations done into opArr
    

    return {dp[strA.size()][strB.size()], opArr};
}