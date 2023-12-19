#pragma once
#include "graph.h"
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <climits>

//Apply djikstra's algorithm on this graph and return map of node to distance of that node from rootNode
std::unordered_map<int, int> djikstra(Graph& graph, int rootNode)
{
    //Q is a min priority queue (ie, front is aways smallest number), with comparision on path length
    //pair<int, int> -> pair of path length from startNode and node number
    auto Compare = [](std::pair<int, int>& P1, std::pair<int, int>& P2) { return P1.first > P2.first; };
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, decltype(Compare)> Q(Compare);

    Q.push({ 0, rootNode}); //Distance of root from itself is zero.
    
    std::unordered_map<int, int> Shortest_Path_Map;
    for(int node=0; node<graph.getNumberNodes(); node++)
    {
        Shortest_Path_Map.insert({node, INT_MAX});
    }

    Shortest_Path_Map.at(rootNode) = 0; //distance to itself

    while (!Q.empty())
	{
        //take front, ie the node with least distance
        std::pair<int, int> temp = Q.top(); Q.pop();
        
        //interate through neighbors of the poped node
        for(int node=0; node<graph.getNumberNodes(); node++)
        {
            if(node == temp.second || !graph.hasEdge(temp.second, node))
            {
                continue;
            }

            //We add the distance till now from rootNode to distance to node
            int tentative_distance = temp.first + graph.getEdgeWeight(temp.second, node);

            //Greedily take the shortest distance
            if (tentative_distance < Shortest_Path_Map.at(node))
			{
				Q.push({ tentative_distance, node }); 
				Shortest_Path_Map.at(node) = tentative_distance;
			} 
        }
    }

    return Shortest_Path_Map;
}