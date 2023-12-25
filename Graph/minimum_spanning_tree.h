#pragma once
#include "graph.h"
#include <queue>
#include <functional>
#include <unordered_set>

//We use prim's algorhitm to get minimum cost of visiting each node in a min spanning tree

struct MST
{
    int weight;
    std::shared_ptr<Graph> mst;

    MST(int wt, std::shared_ptr<Graph> g): weight(wt), mst(g)
    {
    }
};

MST primMST(Graph* graph)
{
    using int_pair = std::pair<int, int>;

    //Q is a min priority queue (ie, front is aways smallest number), with comparision on path length
    //pair<int, int> -> pair of path length from startNode and node number
    auto Compare = [](int_pair& P1, int_pair& P2) { return P1.first > P2.first; };
    std::priority_queue<int_pair, std::vector<int_pair>, decltype(Compare)> Q(Compare);

    std::unordered_set<int> included_nodes; //A set of nodes already included in the MST

    // To store parent array which in turn store MST, index - value represents one edge
    std::vector<int> parent(graph->getNumberNodes(), -1);

    //weight of edge refered by parent array at same index
    std::vector<int> weights(graph->getNumberNodes(), INT_MAX);

    Q.push({ 0, 0}); //Distance of root from itself is zero. We are starting from node 0
    weights[0] = 0;
 
    while (!Q.empty())
    {
        //take front, ie the node with least distance
        int_pair temp = Q.top(); Q.pop();

        included_nodes.insert(temp.second);

        //interate through neighbors of the poped node
        for(auto neighbor : graph->getNeighbors(temp.second))
        {
            if(included_nodes.find(neighbor) != included_nodes.end()) {
                continue;
            }

            int wt = graph->getEdgeWeight(temp.second, neighbor);
            if(wt < weights[neighbor])
            {
                weights[neighbor] = wt;
                Q.push({ wt, neighbor }); 
                parent[neighbor] = temp.second;
            }
        }
    }

    //Create MST 
    std::shared_ptr<Graph> mst = std::make_shared<GraphList>(graph->getNumberNodes(), graph->isDirected());
    int wt_mst(0); //Weight of all the edges in the MST
    for(std::size_t i=0; i<parent.size(); i++)
    {
        if(parent[i] < 0) {
            continue;
        }

        mst->addEdge(parent[i], i, weights[i]);
        wt_mst +=  weights[i];
    }

    return MST(wt_mst, mst);
}