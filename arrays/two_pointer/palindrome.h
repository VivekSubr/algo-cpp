#pragma once
#include <string>
#include <cmath>
#include <cctype>
#include <iostream>

std::string strip_non_alpha(const std::string& str)
{
    std::string ret;
    for(auto ch : str)
    {
        if((std::isalpha(ch) || isdigit(ch)) && !std::isspace(ch)) ret += std::tolower(ch);
    }

    return ret;
}

bool is_valid_palindrome(const std::string& str)
{
    std::string nStr = strip_non_alpha(str);
    int         mid  = std::floor(nStr.size()/2);

    std::cout<<"is_valid_palindrome "<<nStr<<" mid "<<mid<<"\n";

    int start=0, end = nStr.size() - 1;
    while(start <= mid && end >= mid)
    {
        if(nStr.at(start) != nStr.at(end)) 
        {
            std::cout<<"mismatch "<<nStr<<" "<<start<<" "<<end<<"\n";
            return false;
        }

        start++;
        end--;
    }

    return true;
}