#pragma once
#include <vector>
#include <string>
#include <stack>
#include <unordered_map>
#include <unordered_set>

std::stack<char> inflix2postflix(const std::string& expr)
{
    static std::unordered_set<char> op_set = {'*', '/', '+', '-'};

    std::stack<char> ret;
    for (auto ch : expr)
    {
        if(ch == ' ') continue;

        if(op_set.find(ch) != op_set.end()) ret.push(ch);
        else if(ch == '(')                  ret.push(ch);
        else if(ch == ')')
        { //pop the stack and output it until a ‘(‘ is encountered, and discard both the parenthesis. 
          std::vector<char> temp;
          while(ret.top() != '(')
          {
            temp.push_back(ret.top()); 
            ret.pop();
          }

          ret.pop();
          for(auto rch : temp) ret.push(rch);
        }
    }

    return ret;
}

std::string stack2str(std::stack<char>& s)
{
    std::string ret;
    while(!s.empty()) 
    {
        ret += s.top();
        s.pop();
    }

    return ret;
}

int eval(const std::stack<char>& postFlix)
{
    int ret;
    std::stack<char>  operand_stack;
    std::stack<char>  operator_stack; 
    static std::unordered_set<char> op_set = {'*', '/', '+', '-'};

    while(!postFlix.empty())
    {
        char cur = postFlix.top();
        if(op_set.find(cur) != op_set.end())
        {
            operator_stack.push(cur);
        }
        else if(cur == '(')
        {

        }
        else operand_stack.push(cur);
    }

    return ret;
}