#pragma once
#include <string>
#include <unordered_map>
#include <iostream>
#include <cctype>

//https://leetcode.com/problems/regular-expression-matching/description/
/*

Given an input string s and a pattern p, implement regular expression matching with support for '.' and '*' where:

'.' Matches any single character.​​​​
'*' Matches zero or more of the preceding element.
The matching should cover the entire input string (not partial).

*/

class Regex
{
public:
    Regex(const std::string& reg): m_regex(reg)
    {
    }

    bool match(const std::string& str)
    {
        std::size_t str_index   = 0;
        std::size_t regex_index = 0;
        while(str_index < str.size() && regex_index < m_regex.size())
        {
            regex_element ele;
            if(std::isalnum(m_regex[regex_index]))
            {
                ele = regex_element::Character;
            }
            else 
            {
                try { ele = m_regex_elements.at(m_regex[regex_index]); }
                catch(...)
                {
                    std::cout<<"Invalid regex character "<<m_regex[regex_index]<<"\n";
                    return false;
                }
            }

            switch(ele)
            {
                case regex_element::Dot:
                    if(isNextRegexEleStar(regex_index)) {
                        str_index = match_any_star(str_index, str);
                        ++regex_index;
                    }
                    else str_index = match_any(str_index, str);
                    break;

                case regex_element::Character:
                    if(isNextRegexEleStar(regex_index)) {
                        str_index = match_characters(str_index, m_regex[regex_index], str);
                        ++regex_index;
                    }
                    else str_index = match_character(str_index, m_regex[regex_index], str);
                    break;

                default:
                    std::cout<<"Unhandled regex_element\n";
                    return false;
            }

            ++regex_index;
            if(str_index == std::string::npos) return false;
        }

        //regex must consume the entire string, and entire regex must be used to be considered a match
        return (str_index == str.size()) && (regex_index == m_regex.size()); 
    }

private:
    std::string m_regex;

    enum class regex_element 
    {
        Dot = 0,
        Star, 
        Wildcard,
        Character
    };

    std::unordered_map<char, regex_element> m_regex_elements = {
        {'.', regex_element::Dot}, {'*', regex_element::Star}, {'?', regex_element::Wildcard}
    };

    bool isNextRegexEleStar(std::size_t i)
    {
        if(i == m_regex.size() - 1) return false;
        if(m_regex[i+1] == '*')     return true;
        return false;
    }

    bool isNextRegexEleWildcard(std::size_t i)
    {
        if(i == m_regex.size() - 1) return false;
        if(m_regex[i+1] == '?')     return true;
        return false;
    }
    
    std::size_t match_character(std::size_t i, char ch, const std::string& str)
    {
        if(i >= str.size() || str[i] != ch) return std::string::npos;
        return ++i;
    }

    std::size_t match_characters(std::size_t i, char ch, const std::string& str)
    {
        while(i < str.size() && str[i] == ch) ++i;
        return i;
    }

    std::size_t match_any(std::size_t i, const std::string& str)
    {
        if(i < str.size()) ++i;
        return i;
    }

    std::size_t match_any_star(std::size_t i, const std::string& str)
    {
        return str.size(); //match till end
    }
};

