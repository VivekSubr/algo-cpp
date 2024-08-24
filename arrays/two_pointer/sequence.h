#pragma once
#include <string>

bool is_sub_sequence(const std::string& str, std::string& sub_str)
{
    if(sub_str.size() == 0) return true;
    if(sub_str.size() == 1)
    {
        char ch = sub_str.at(0);
        for(auto ele : str)
        {
            if(ele == ch) return true;
        }

        return false;
    }

    int i_str(0), i_sub_str(0);
    while(i_str < str.size() && i_sub_str < sub_str.size())
    {
        if(str.at(i_str) == sub_str.at(i_sub_str))
        {
            ++i_sub_str;
        }

        if(i_sub_str == sub_str.size() - 1) return true;

        ++i_str;
    }

    return false;
}