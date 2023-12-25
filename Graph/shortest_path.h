#pragma once
#include "graph.h"
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <climits>

//Apply djikstra's algorithm on this graph and return map of node to distance of that node from rootNode
std::unordered_map<int, int> djikstra(Graph* graph, int rootNode)
{
    using int_pair = std::pair<int, int>;

    //Q is a min priority queue (ie, front is aways smallest number), with comparision on path length
    //pair<int, int> -> pair of path length from startNode and node number
    auto Compare = [](int_pair& P1, int_pair& P2) { return P1.first > P2.first; };
    std::priority_queue<int_pair, std::vector<int_pair>, decltype(Compare)> Q(Compare);

    Q.push({ 0, rootNode}); //Distance of root from itself is zero.
    
    std::unordered_map<int, int> Shortest_Path_Map;
    for(int node=0; node<graph->getNumberNodes(); node++)
    {
        Shortest_Path_Map.insert({node, INT_MAX});
    }

    Shortest_Path_Map.at(rootNode) = 0; //distance to itself

    while (!Q.empty())
	{
        //take front, ie the node with least distance
        int_pair temp = Q.top(); Q.pop();
        
        //interate through neighbors of the poped node
        for(auto neighbor : graph->getNeighbors(temp.second))
        {
            //We add the distance till now from rootNode to distance to node
            int tentative_distance = temp.first + graph->getEdgeWeight(temp.second, neighbor);

            //Greedily take the shortest distance
            if (tentative_distance < Shortest_Path_Map.at(neighbor))
			{
				Q.push({ tentative_distance, neighbor }); 
				Shortest_Path_Map.at(neighbor) = tentative_distance;
			} 
        }
    }

    return Shortest_Path_Map;
}