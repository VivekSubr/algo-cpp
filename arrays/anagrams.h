#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

/*
Given an array of strings strs, group all anagrams together into sublists. 
You may return the output in any order.

An anagram is a string that contains the exact same characters as another string, 
but the order of the characters can be different.

Example 1:
Input: strs = ["act","pots","tops","cat","stop","hat"]

Output: [["hat"],["act", "cat"],["stop", "pots", "tops"]]

*/


bool are_anagram(const std::string& s, const std::string& t)
{
    if(t.size() != s.size()) return false;
    std::unordered_set<char> s_set, t_set;
    for(auto sch : s)  { s_set.insert(sch); }
    for(auto tch : t)  { t_set.insert(tch); }

    return s_set.size() == t_set.size();
}

std::vector<std::vector<std::string>> groupAnagrams(std::vector<std::string>& strs) 
{
    /*
        We need group strings into buckets, where they are all anagrams of each other.
        Basically, two options:
            1. sort the strings and hash them - anagrams have the same hash
            2. Union Find - Treat each string as a node; union two strings if they're anagrams 
               of each other. Connected components = buckets.
    */

    std::vector<std::vector<std::string>> ret;
    std::unordered_map<std::string, std::vector<std::string>> anagram_map;
    for(auto str : strs)
    {

    }

    return ret;
}