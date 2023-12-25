#pragma once
#include "graph.h"
#include <deque>
#include <unordered_set>

//Do a BFS on this graph, starting at give node, and return array of visited nodes in order
std::vector<int> bfs(Graph* graph, int startNode)
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
        for(auto neighbor : graph->getNeighbors(node))
        {
            if(set_visted.find(neighbor) == set_visted.end())
            { //if this neighbor node is not visited already
              bfs_Q.push_back(neighbor);
              set_visted.insert(neighbor);
              bfs_node_visited_arr.push_back(neighbor);
            }
        }
    }

    return bfs_node_visited_arr;
}

void do_dfs(Graph* graph, std::unordered_set<int>& set_visited, std::vector<int>& arr_visited, int node)
{
    //visit all neighbors 
    for(auto neighbor : graph->getNeighbors(node))
    {
        if(set_visited.find(neighbor) == set_visited.end())
        {
            set_visited.insert(neighbor);
            arr_visited.push_back(neighbor);
            do_dfs(graph, set_visited, arr_visited, neighbor);
        }
    }
}

//Do a DFS on this graph, starting at give node, and return array of visited nodes in order
std::vector<int> dfs(Graph* graph, int startNode)
{
    std::unordered_set<int> set_visited; //a set of visited nodes
    std::vector<int>        dfs_node_visited_arr;

    do_dfs(graph, set_visited, dfs_node_visited_arr, startNode);
    return dfs_node_visited_arr;
}


bool do_hasCycle(Graph* graph, std::unordered_set<int>& set_visited, std::unordered_set<int>& set_visiting, int node)
{
    if(set_visited.find(node) == set_visited.end())
    {
        set_visiting.insert(node);
        set_visited.insert(node);
        
        for(auto neighbor : graph->getNeighbors(node))
        {
            if(do_hasCycle(graph, set_visited, set_visiting, neighbor))
            {
                return true;
            }
        
            if(set_visiting.find(neighbor) != set_visiting.end())
            {
                return true; //found a node again in the same path, cycle!
            }
        }
    }
    else if(set_visiting.find(node) != set_visiting.end())
    {
        return true;
    }

    set_visiting.erase(node); //remove from visiting since we've done dfs on all it's children
    return false;
}

//There is a cycle in a graph only if there is a back edge [i.e., a node points to one of its ancestors] present in the graph.
//To detect a back edge, we need to keep track of the nodes visited till now and the nodes that are in the current recursion stack 
//[i.e., the current path that we are visiting]. If during recursion, we reach a node that is already in the recursion stack, 
//there is a cycle present in the graph.
bool hasCycle(Graph* graph)
{
    //A set of nodes who dfs has returned, ie there descendants have no cycle
    std::unordered_set<int> set_visisted;  
    
    //keep track of dfs going deep with this stack, push node into set before calling dfs and remove once dfs returns
    std::unordered_set<int> set_visiting;

    for(int node = 0; node < graph->getNumberNodes(); node++)
    { //Need to do this for all nodes, as graph may be disjoint
        if((set_visisted.find(node) == set_visisted.end()) && do_hasCycle(graph, set_visisted, set_visiting, node)) 
        {
            return true;
        }
    }

    return false;
}