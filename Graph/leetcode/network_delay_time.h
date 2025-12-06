#pragma once
#include <vector>
#include <queue>
#include <unordered_map>
#include <functional>
#include <climits>
//leetcode.com/problems/network-delay-time/description/

//We are given graph in list form here... 
//You are given a network of n nodes, labeled from 1 to n. You are also given times, a list of travel times as directed edges times[i] = (ui, vi, wi)
//where ui is the source node, vi is the target node, and wi is the time it takes for a signal to travel from source to target.
int networkDelayTime(const std::vector<std::vector<int>>& times, int, int k) 
{
    using int_pair = std::pair<int, int>; //fi

    //Q is a max priority queue (ie, front is aways largest number), with comparision on path length
    //pair<int, int> -> pair of path length from startNode and node number
    auto Compare = [](int_pair& P1, int_pair& P2) { return P1.first < P2.first; };
    std::priority_queue<int_pair, std::vector<int_pair>, decltype(Compare)> Q(Compare);  

	Q.push({ 0, k}); //Distance of root from itself is zero.
    
	//This is a map of node and the 
    std::unordered_map<int, int> Longest_Path_Map;
	for(size_t i=0; i<times.size(); i++)
	{
		Longest_Path_Map.insert({i, INT_MIN});
	}

    Longest_Path_Map.at(k) = 0; //distance to itself
    int max_distance(0);
    while (!Q.empty())
	{
        //take front, ie the node with least distance
        int_pair temp = Q.top(); Q.pop();
        
        //interate through neighbors of the poped node
        for(size_t i=0; i<times.size(); i++)
        {
            if(times[i][0] != temp.second) continue; //not a neighbor

            std::vector<int> neighbor = times[i]; //neighbor is in form {source, dest, weight}
            
            //We add the distance till now from rootNode to distance to node
            int tentative_distance = temp.first + neighbor.at(2);

            //Greedily take the longest distance
            if (tentative_distance > Longest_Path_Map.at(neighbor.at(1)))
			{
				Q.push({ tentative_distance, neighbor.at(1) }); 
				Longest_Path_Map.at(neighbor.at(1)) = tentative_distance;
				
				if(tentative_distance > max_distance) max_distance = tentative_distance;
			}
        }
    }
	
	return max_distance;
}