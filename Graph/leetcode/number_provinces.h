#pragma once
#include <vector>
#include <unordered_set>
#include <unordered_map>

void dfs(int node, const std::vector<std::vector<int>>& graph, std::unordered_set<int>& set_visited)
{
    if(set_visited.find(node) != set_visited.end())
    {
        return; //already visited
    }

    set_visited.insert(node);
    for(size_t neighbor=0; neighbor<graph.size(); neighbor++)
    {
        if(graph[node][neighbor] == 1 && set_visited.find(neighbor) == set_visited.end())
        {
            dfs(neighbor, graph, set_visited);
        }
    }
}

int number_provinces(const std::vector<std::vector<int>>& isConnected) 
{
    int nProvinces(0);
    std::unordered_map<int, int> node_province_map;
    for(size_t i=0; i<isConnected.size(); i++)
    {
        if(node_province_map.find(i) != node_province_map.end())
        {
            continue; //this node is already assigned to a province
        }

        nProvinces++;
        std::unordered_set<int> set_visited;
        dfs(i, isConnected, set_visited);

        for(auto node : set_visited)
        {
            node_province_map.emplace(node, i);
            node_province_map.emplace(i, node);
        }
    }

    return nProvinces;
}