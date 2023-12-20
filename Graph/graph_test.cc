#include "graph.h"
#include "graph_traversal.h"
#include "shortest_path.h"
#include "topological_sort.h"
#include "graph_test.h"

/*TEST_F(TestGraph, BFS)
{
    Graph g(4);
    g.addEdges({ { 0, 1, 1 }, { 0, 2, 1 }, { 2, 0, 1 }, { 2, 3, 1 } });

    auto bfs_arr = bfs(g, 2);

    ASSERT_EQ(bfs_arr.size(), 4);

    ASSERT_EQ(bfs_arr[0], 2);
    ASSERT_EQ(bfs_arr[1], 0);
    ASSERT_EQ(bfs_arr[2], 1);
    ASSERT_EQ(bfs_arr[3], 3);
}*/

TEST_F(TestGraph, Djikstra)
{
    Graph g(5);
    g.addEdges({ { 0, 1, 1 }, { 0, 2, 4 }, { 1, 2, 1 }, { 3, 4, 3 } });

    auto ShortestPaths = djikstra(g, 0);

	ASSERT_EQ(ShortestPaths.size(), 5);
	ASSERT_EQ(ShortestPaths.at(0),  0);
	ASSERT_EQ(ShortestPaths.at(1),  1);
	ASSERT_EQ(ShortestPaths.at(2),  2);
	ASSERT_EQ(ShortestPaths.at(3),  INT_MAX);
	ASSERT_EQ(ShortestPaths.at(4),  INT_MAX);
}

TEST_F(TestGraph, inDegree)
{
    Graph g(5, true, { {0, 1, 1}, {1, 2, 1}, {1, 3, 1}, {2, 3, 1}, {2, 4, 1}, {3, 4, 1}});

    ASSERT_EQ(g.inDegree(0), 0);
    ASSERT_EQ(g.inDegree(1), 1);
    ASSERT_EQ(g.inDegree(2), 1);
    ASSERT_EQ(g.inDegree(3), 2);
    ASSERT_EQ(g.inDegree(4), 2);

    g.deleteNode(0);
    ASSERT_EQ(g.inDegree(1), 0);
}

TEST_F(TestGraph, HasCycle)
{
    //Cycle here, 0 -> 2 -> 0
   // Graph g(4, true, { {0, 1, 1}, {0, 2, 1}, {1, 2, 1}, {2, 3, 1}, {2, 0, 1} });
   // ASSERT_TRUE(hasCycle(g));

    Graph g2(4, true, { {0, 1, 1}, {0, 2, 1}, {1, 2, 1}, {2, 3, 1} });
    ASSERT_FALSE(hasCycle(g2));
}

TEST_F(TestGraph, TopologicalSort)
{
    Graph g(5, true, { {0, 1, 1}, {1, 2, 1}, {1, 3, 1}, {2, 3, 1}, {2, 4, 1}, {3, 4, 1}});
    
    auto sortArr = topologicalSort(g);
    ASSERT_EQ(sortArr.size(), 5);

    ASSERT_EQ(sortArr[0], 0);
    ASSERT_EQ(sortArr[1], 1);
    ASSERT_EQ(sortArr[2], 2);
    ASSERT_EQ(sortArr[3], 3);
    ASSERT_EQ(sortArr[4], 4);
}