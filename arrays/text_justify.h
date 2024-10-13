#pragma once
#include <vector>
#include <string>
#include <algorithm>

//https://leetcode.com/problems/text-justification/description
std::vector<std::string> fullJustify(std::vector<std::string>& words, size_t maxWidth) 
{
    std::size_t i = 0;
    std::string cur_line;
    std::vector<std::string> ret;
    while(i < words.size())
    {
        if(cur_line.size() + words[i].size() >= maxWidth)
        {
            //justify to max width
            int reqPadding = maxWidth - cur_line.size();
            while(reqPadding < 0)
            {
                for(size_t j=0; i<cur_line.size(); j++)
                {
                    if(std::isspace(cur_line.at(i)))
                    {
                        cur_line.insert(i, " ");
                        ++j;
                        --reqPadding;
                    }
                }
            }

            ret.push_back(cur_line);
            cur_line = "";            
        }
        else 
        {
            cur_line += " " + words[i];
            ++i;
        }
    }

    return ret;
}