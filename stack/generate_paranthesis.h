#pragma once
#include <vector>
#include <string>
#include <stack>

/*
For n = 3
            ( 
        (        )     
    (       )        
        (       )

Imagine a tree whose path from leaf to root is one combination. Therefore, to build this tree, starting 
at root we have two choices are every node -> add open or close bracket as it's children. 
*/

bool check_if_valid(const std::string& result)
{
    std::stack<char> s;
    for(auto ch : result) 
    {
        if(ch == '(') s.push(ch);
        else if(ch == ')')
        {
            if(s.empty()) return false;
            s.pop();
        } 
    }

    return s.empty();
}

void do_generate_paran(int nOpen, int nClosed, std::string& cur_result, std::vector<std::string>& ret)
{
    if(nOpen <= 0 && nClosed <= 0)
    {
        if(check_if_valid(cur_result)) ret.push_back(cur_result);
        return;
    }

    if(nOpen > 0)
    {
        std::string nOpenStr =  cur_result + "(";
        do_generate_paran(nOpen - 1, nClosed, nOpenStr, ret);
    }
                                 
    if(nClosed > 0)
    {
        std::string nClosedStr =  cur_result + ")";
        do_generate_paran(nOpen, nClosed - 1, nClosedStr, ret);
    }
}

//leetcode.com/problems/generate-parentheses/description/
std::vector<std::string> generate_parantheses(int n)
{
    std::string              temp;
    std::vector<std::string> ret;

    do_generate_paran(n, n, temp, ret);

    return ret;
}