#pragma once
#include <vector>
#include <string>
#include <unordered_set>
#include <iostream>

//leetcode.com/problems/word-search/description/

class WordSearch
{
public:
    static bool exists(const std::vector<std::vector<char>>& board, const std::string& word) 
    {
        std::unordered_set<std::string> memo_set;
        return search(board, 0, 0, 0, word, memo_set);
    }

private:
    static bool search(const std::vector<std::vector<char>>& board, int i, int j, int nChar, const std::string& word, 
                       std::unordered_set<std::string>& memo_set)
    {
        if(memo_set.find(stringify(i, j)) != memo_set.end()) return false;

        if(nChar == word.size()) return true;

        if(i >= board.size() || j >= board[0].size() || i < 0 || j < 0) 
        {
            return false;
        }

        //std::cout<<"("<<i<<" "<<j<<" "<<board[i][j]<<") ";
        memo_set.insert(stringify(i, j));
        if(board[i][j] == word[nChar])
        {
            if(search(board, i+1, j,   nChar+1, word, memo_set) || 
               search(board, i,   j+1, nChar+1, word, memo_set) || 
               search(board, i-1, j,   nChar+1, word, memo_set) || 
               search(board, i, j-1,   nChar+1, word, memo_set))
            {
                return true;
            }
        }

        std::cout<<"\n";
        return false;
    }

    static std::string stringify(int i, int j)
    {
        return std::to_string(i) + "-" + std::to_string(j);
    }
};