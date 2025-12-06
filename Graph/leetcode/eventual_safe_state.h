#pragma once
#include <vector>
#include <unordered_set>
//leetcode.com/problems/find-eventual-safe-states/

void find_safe_nodes(int node,const std::vector<std::vector<int>>& graph,
                    std::vector<int>& safe_nodes,
                    std::unordered_set<int>& visited_nodes)
{
    if(visited_nodes.find(node) != visited_nodes.end())
    {
        return; //already visited
    }

    visited_nodes.insert(node);
    size_t non_safe_nodes = graph[node].size();
    for(auto neighbor : graph[node])
    {
       //check if all neighbors are safe or terminal nodes
	   if(graph[neighbor].size() == 0 ||  //terminal node
	      std::find(safe_nodes.begin(), safe_nodes.end(), neighbor) != safe_nodes.end()) 
	   {
		   --non_safe_nodes;	  
		   continue;
	   }
       
       find_safe_nodes(neighbor, graph, safe_nodes, visited_nodes);
    }
	
	if(non_safe_nodes == 0) safe_nodes.push_back(node);
}

//Here graph is given in list form,
//eg: graph = [[1,2],[2,3],[5],[0],[5],[],[]]... here 0 are outbound connection to 1 and 2.
std::vector<int> eventualSafeNodes(std::vector<std::vector<int>>& graph) 
{
    std::unordered_set<int>  visited_nodes;
    std::vector<int>         safe_nodes;

    for(size_t node=0; node<graph.size(); node++)
    {
        if(std::find(safe_nodes.begin(), safe_nodes.end(), node) != safe_nodes.end())
        {
            continue; //already known safe or terminal node
        }

        find_safe_nodes(node, graph, safe_nodes, visited_nodes);
    }

    return safe_nodes;
}