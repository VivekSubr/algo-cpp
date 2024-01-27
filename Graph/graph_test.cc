#include "graph.h"
#include "graph_traversal.h"
#include "shortest_path.h"
#include "topological_sort.h"
#include "minimum_spanning_tree.h"
#include "union_find.h"
#include "graph_test.h"

TEST_F(TestGraph, GraphImpl)
{
    std::vector<std::shared_ptr<Graph>> testArr;
    testArr.push_back(CreateGraph(5, false,  GraphRepresentation::AdjMatrix));
    testArr.push_back(CreateGraph(5, true,   GraphRepresentation::AdjMatrix));
    testArr.push_back(CreateGraph(5, false,  GraphRepresentation::AdjList));
    testArr.push_back(CreateGraph(5, true,   GraphRepresentation::AdjList));

    std::vector<std::tuple<int, int, int>> edges = { {0, 1, 1}, {1, 2, 1}, {1, 3, 1}, {2, 3, 1}, {2, 4, 1}, {3, 4, 1} };
    std::vector<int> neighbors1 = {0, 2, 3};

    for(auto g : testArr)
    {
        g->addEdges(edges);
        ASSERT_EQ(g->getNumberNodes(), 5);

        auto neighbors = g->getNeighbors(1);
        std::vector<int> neighbors1;
        if(g->isDirected()) neighbors1 = {2, 3};
        else                neighbors1 = {0, 2, 3};

        ASSERT_EQ(neighbors.size(), neighbors1.size()) << g->getName();

        int nFound(0);
        for(auto n : neighbors)
        {
            for(auto tn : neighbors1)
            {
                if(tn == n) {
                    ++nFound;
                    break;
                }
            }
        }

        ASSERT_EQ(nFound, neighbors1.size());
 
        if(g->isDirected())
        {
            ASSERT_EQ(g->inDegree(0), 0) << g->getName();
            ASSERT_EQ(g->inDegree(1), 1);
            ASSERT_EQ(g->inDegree(2), 1);
            ASSERT_EQ(g->inDegree(3), 2);
            ASSERT_EQ(g->inDegree(4), 2);

            g->deleteNode(0);
            ASSERT_EQ(g->inDegree(1), 0);
        }
    }
}

TEST_F(TestGraph, Search)
{
    GraphList g(4);
    g.addEdges({ { 0, 1, 1 }, { 0, 2, 1 }, { 2, 0, 1 }, { 2, 3, 1 } });

    auto bfs_arr = bfs(&g, 2);
    ASSERT_EQ(bfs_arr.size(), 4);

    auto dfs_arr = dfs(&g, 2);
    ASSERT_EQ(dfs_arr.size(), 4);
}

TEST_F(TestGraph, Djikstra)
{
    GraphMatrix g(5);
    g.addEdges({ { 0, 1, 1 }, { 0, 2, 4 }, { 1, 2, 1 }, { 3, 4, 3 } });

    auto ShortestPaths = djikstra(&g, 0);

	ASSERT_EQ(ShortestPaths.size(), 5);
	ASSERT_EQ(ShortestPaths.at(0),  0);
	ASSERT_EQ(ShortestPaths.at(1),  1);
	ASSERT_EQ(ShortestPaths.at(2),  2);
	ASSERT_EQ(ShortestPaths.at(3),  INT_MAX);
	ASSERT_EQ(ShortestPaths.at(4),  INT_MAX);
}

TEST_F(TestGraph, HasCycle)
{
    //Cycle here, 0 -> 2 -> 0
    GraphList g(4, true, { {0, 1, 1}, {1, 2, 1}, {0, 2, 1}, {2, 0, 1}, {2, 3, 1} });
    auto n0 = g.getNeighbors(0);
    ASSERT_EQ(n0.size(), 2); 

    ASSERT_TRUE(hasCycle(&g));

    GraphList g2(4, true, { {0, 1, 1}, {0, 2, 1}, {1, 2, 1}, {2, 3, 1} });
    ASSERT_FALSE(hasCycle(&g2));
}

TEST_F(TestGraph, TopologicalSort)
{
    GraphList g(5, true, { {0, 1, 1}, {1, 2, 1}, {1, 3, 1}, {2, 3, 1}, {2, 4, 1}, {3, 4, 1}});
    ASSERT_FALSE(hasCycle(&g));

    auto sortArr = topologicalSort(&g);
    ASSERT_EQ(sortArr.size(), 5);

    ASSERT_EQ(sortArr[0], 0);
    ASSERT_EQ(sortArr[1], 1);
    ASSERT_EQ(sortArr[2], 2);
    ASSERT_EQ(sortArr[3], 3);
    ASSERT_EQ(sortArr[4], 4);
} 

TEST_F(TestGraph, MinimalSpanningTree)
{ 
    //https://www.geeksforgeeks.org/prims-minimum-spanning-tree-mst-greedy-algo-5/
    std::vector<std::tuple<int, int, int>> graph_edges = 
    {
        {0, 1, 4},  {0, 7, 8}, 
        {1, 7, 11}, {1, 2, 8}, 
        {2, 8, 2},  {2, 3, 7}, {2, 5, 4}, {3, 4, 9},
        {3, 5, 14},  
        {4, 5, 10},
        {5, 6, 2}, 
        {6, 8, 6}, {6, 7, 1},
        {7, 8, 7},
    };

    GraphList g(9, false, graph_edges);
    
    for(auto edge : graph_edges)
    {
        ASSERT_EQ(g.getEdgeWeight(std::get<0>(edge), std::get<1>(edge)), std::get<2>(edge));
        ASSERT_EQ(g.getEdgeWeight(std::get<1>(edge), std::get<0>(edge)), std::get<2>(edge));
    }

    auto MST = primMST(&g);
    ASSERT_EQ(MST.weight, 37);
    ASSERT_TRUE(isConnected(MST.mst));
}

TEST_F(TestGraph, CountConnectedComponents)
{ 
    auto g = CreateGraph(5, false,  GraphRepresentation::AdjList);
    g->addEdges({ {0, 1, 1}, {1, 2, 1}, {3, 4, 1}});

    DisjointSet ds(g);
    ASSERT_EQ(2, ds.count_connected_components());
}