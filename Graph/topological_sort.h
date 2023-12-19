#pragma once
#include "graph.h"
#include "graph_traversal.h"
#include <unordered_set>

//The topological sort algorithm takes a directed graph and returns an array of the nodes 
//where each node appears before all the nodes it points to.
//Graphs with cycles do not have a topological order


//Node of indegree of 0 will be the first in the sorted order
//Remove the sorted node from graph
//Find nodes with indegree 0 and insert into order
//Repeat till done

std::vector<int> topologicalSort(Graph& graph)
{
    std::vector<int>        ret;
    std::unordered_set<int> visited_set;
    if(hasCycle(graph)) {
        return ret;
    }
    
    size_t nNodes = graph.getNumberNodes();
    while(ret.size() < nNodes) 
    {
        for(size_t node=0; node<nNodes; node++)
        {
            if((visited_set.find(node) == visited_set.end()) && (graph.inDegree(node) == 0))
            {
                ret.push_back(node);
                visited_set.insert(node);
                graph.deleteNode(node);
                break;
            }
        }
    }

    return ret;
}