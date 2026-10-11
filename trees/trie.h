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

	    TrieNode(char ch): val(ch) {}
    };

    Trie(const std::vector<std::string>& words)
    {
	    for(auto word : words) 
	    {
	        addWordImpl(word);
	    }
    }

    void addWord(const std::string& str)
    {
	    addWordImpl(str);
    }

    bool find(const std::string& str)
    {
        TrieNode& node = m_root;
	    for(auto ch : str)
	    {
	        auto nodeIt = node.children.find(ch);
	        if(nodeIt == node.children.end()) return false;
	   
	        node = nodeIt->second;
	    }

	    return true;
    }

    std::vector<std::string> prefix(const std::string& prefix)
    {
	    std::vector<std::string> ret;
	    TrieNode& node = m_root;

	    for(auto ch : prefix)
	    {
	        auto nodeIt = node.children.find(ch);
	        if(nodeIt == node.children.end()) return ret;

	        node = nodeIt->second;
	    }

	    gatherWords(prefix, node, ret);
	    return ret;
    }

private:
    TrieNode m_root{'\0'};

    void addWordImpl(const std::string& str)
    {
        TrieNode* node = &m_root;
	    for(size_t i=0; i<str.size(); i++)
	    {
	        auto nodeIt = node->children.find(str.at(i));
            if(nodeIt == node->children.end())
	        {
	            addNodes(std::string(str.begin() + i, str.end()), node);
	            return; 
	        }

	        node = &nodeIt->second;
	    }
    }

    void addNodes(const std::string& str, TrieNode* node)
    {
	    for(auto ch : str)
	    {
	        TrieNode new_node(ch);
	        node->children.insert({ch, new_node});

	        node = &node->children.at(ch);
	    }
    }

    void gatherWords(const std::string& prefix, TrieNode& node, std::vector<std::string>& ret)
    {
	    if(node.children.size() == 0) ret.push_back(prefix);
	    for(auto& child : node.children)
	    {
	        std::string new_prefix = prefix + child.first;
	        gatherWords(new_prefix, child.second, ret);
	    }
    }
};