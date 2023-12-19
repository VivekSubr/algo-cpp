#pragma once
#include "graph.h"
#include <deque>
#include <unordered_set>

//Do a BFS on this graph, starting at give node, and return array of visited nodes in order
std::vector<int> bfs(const Graph& graph, int startNode)
{
    //To make this process easy, use a queue to store the node and mark it as 'visited' until all its neighbours 
    //(vertices that are directly connected to it) are marked. The queue follows the First In First Out (FIFO) queuing method, 
    //and therefore, the neigbors of the node will be visited in the order in which they were inserted in the node i.e. 
    //the node that was inserted first will be visited first, and so on.
    std::deque<int> bfs_Q;

    //a set of visited nodes
    std::unordered_set<int> set_visted;
    std::vector<int>        bfs_node_visited_arr;

    bfs_Q.push_back(startNode);
    set_visted.insert(startNode);
    bfs_node_visited_arr.push_back(startNode);

    while(!bfs_Q.empty())
    {
        int node = bfs_Q.at(0);
        bfs_Q.pop_front();

        //visit all neighbors of dequed node
        for(int j=0; j<graph.getNumberNodes(); j++)
        {
            if(j == node || !graph.hasEdge(node, j)) {
                continue; //no path from node to j
            }

            if(set_visted.find(j) == set_visted.end())
            { //if this neighbor node is not visited already
              bfs_Q.push_back(j);
              set_visted.insert(j);
              bfs_node_visited_arr.push_back(j);
            }
        }
    }

    return bfs_node_visited_arr;
}

void do_dfs(const Graph& graph, std::unordered_set<int>& set_visited, std::vector<int>& arr_visited, int node)
{
    //visit all neighbors 
    for(int j=0; j<graph.getNumberNodes(); j++)
    {
        if(j == node || !graph.hasEdge(node, j)) {
            continue; //no path from node to j
        }

        if(set_visited.find(j) == set_visited.end())
        {
            set_visited.insert(j);
            arr_visited.push_back(j);
            do_dfs(graph, set_visited, arr_visited, j);
        }
    }
}

//Do a DFS on this graph, starting at give node, and return array of visited nodes in order
std::vector<int> dfs(const Graph& graph, int startNode)
{
    std::unordered_set<int> set_visited; //a set of visited nodes
    std::vector<int>        dfs_node_visited_arr;

    do_dfs(graph, set_visited, dfs_node_visited_arr, startNode);
    return dfs_node_visited_arr;
}

bool do_hasCycle(const Graph& graph, std::unordered_set<int>& set_visited)
{
    //visit all neighbors 
    for(int j=0; j<graph.getNumberNodes(); j++)
    {
        if(j == node || !graph.hasEdge(node, j)) {
            continue; //no path from node to j
        }

        if(set_visited.find(j) == set_visited.end())
        {
            set_visited.insert(j);
            do_dfs(graph, set_visited, arr_visited, j);
        }
        else return true;
    }

    return false;
}

bool hasCycle(const Graph& graph)
{
    std::unordered_set<int> set_visited; //a set of visited nodes

    return do_hasCycle(graph, set_visited);
}
