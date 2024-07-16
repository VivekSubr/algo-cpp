#pragma once
#include <unordered_map>
#include <string>

/*
    A Trie data structure consists of nodes connected by edges. 
    Each node represents a character or a part of a string. 
    The root node, the starting point of the Trie, represents an empty string. Each edge emanating from a node signifies a 
    specific character. The path from the root to a node represents the prefix of a string stored in the Trie.
*/

class Trie 
{
public:
    struct TrieNode 
    {
        char val;
        std::unordered_map<char, TrieNode> children;
    }

    void addWord(const std::string& str)
    {
    }

private:
    std::unordered_map<char, TrieNode> m_root;
};