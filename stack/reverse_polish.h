#pragma once
#include <vector>
#include <string>
#include <stack>
#include <unordered_map>

std::string inflix2postflix(const std::string& expr)
{
    //operator precedence as per BODMAS, lower values more precedence
    static std::unordered_map<char, int> op_set = {{'*', 0}, {'/', 1}, {'+', 2}, {'-', 3}};

    std::string ret;
    std::stack<char> op_stack;
    for (auto ch : expr)
    {
        if(ch == ' ') continue;

        if(op_set.find(ch) != op_set.end()) 
        {
            if(op_stack.size() > 0)
            {
                auto cur = op_stack.top();
                while(op_set.find(cur) != op_set.end() && op_set.at(cur) <= op_set.at(ch))
                { //if the new operator has higher or equal precedence than top
                    ret += cur;
                    op_stack.pop();

                    if(op_stack.size() > 0) cur = op_stack.top();
                    else                    break;
                }
            }

            op_stack.push(ch);
        }
        else if(ch == '(') op_stack.push(ch);
        else if(ch == ')')
        { //pop the stack and output it until a ‘(‘ is encountered, and discard both the parenthesis.      
          while(op_stack.top() != '(')
          {
            ret += op_stack.top();
            op_stack.pop();
          }

          op_stack.pop();
        }
        else ret += ch; //it will be an operand, which are directly added.
    }

    while(op_stack.size() > 0)
    { //append the remaining
        ret += op_stack.top();
        op_stack.pop();
    }

    return ret;
}

int eval(const std::string& postFlix)
{
    return 0;
}