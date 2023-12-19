#pragma once
#include <vector>
#include <tuple>
#include <iostream>

class Graph
{
public:
    Graph(int nodes, bool directed = false): m_nNodes(nodes), m_isDirected(directed)
    {
        m_adjMatrix.resize(m_nNodes, std::vector<int>(m_nNodes, 0));
    }

    Graph(int nodes, bool directed, const std::vector<std::tuple<int, int, int>>& edges): m_nNodes(nodes), m_isDirected(directed)
    {
        m_adjMatrix.resize(m_nNodes, std::vector<int>(m_nNodes, 0));
        addEdges(edges);
    }

    void addEdge(int i, int j, int weight=1)
    {
        m_adjMatrix[i][j] = weight;
        if(!m_isDirected) {
            m_adjMatrix[j][i] = weight;
        }
    }

    void addEdges(const std::vector<std::tuple<int, int, int>>& edges)
    {
        for(const auto& edge: edges)
        {
            addEdge(std::get<0>(edge), std::get<1>(edge), std::get<2>(edge));
        }
    }

    bool hasEdge(int i, int j) const
    {
        return (m_adjMatrix[i][j] > 0);
    }

    int getEdgeWeight(int i, int j) const
    {
        return m_adjMatrix[i][j];
    }

    void deleteNode(int node)
    {
        for(int i=0; i<m_nNodes; i++)
        {
            m_adjMatrix[node][i] = 0;
            m_adjMatrix[i][node] = 0;
        }

    }

    int getNumberNodes() const { return m_nNodes; }

    //Let A be adjacency matrix and At transpose of A, then Multiply A and At, and diagonal elements represent out degree of node
    //And Multiply At and A represents in degree

    int inDegree(int node) 
    {
        if(!m_isDirected) return -1;
        
        auto res = multiply(transpose(m_adjMatrix), m_adjMatrix);
        return res[node][node];
    }

    int outDegree(int node) 
    {
        if(!m_isDirected) return -1;

        auto res = multiply(m_adjMatrix, transpose(m_adjMatrix));
        return res[node][node];
    }

private:
    int                           m_nNodes;
    bool                          m_isDirected;
    std::vector<std::vector<int>> m_adjMatrix; //there can be a map of node_num : data corresponding to node

    std::vector<std::vector<int>> transpose(const std::vector<std::vector<int>>& A)
    {
        std::vector<std::vector<int>> T(A.size(), std::vector<int>(A.size(), 0));
        
        for(std::size_t i=0; i<A.size(); i++)
        {
            for(std::size_t j=0; j<A.size(); j++)
            {
               T[j][i] = A[i][j];
            }
        }

        return T;
    }

    std::vector<std::vector<int>> multiply(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B)
    {
        std::vector<std::vector<int>> ret(A.size(), std::vector<int>(A.size(), 0));
        for(std::size_t i=0; i<A.size(); i++)
        {
            for(std::size_t j=0; j<A.size(); j++)
            {
                for(size_t k=0; k<A.size(); k++)
                {
                    ret[i][j] += A[i][k] * B[k][j];
                }
            }
        }

        return ret;
    }

    void printMatrix(const std::vector<std::vector<int>>& M)
    {
        for(std::size_t i=0; i<M.size(); i++)
        {
            for(std::size_t j=0; j<M.size(); j++)
            {
                std::cout<<M[i][j]<<" ";
            } 

            std::cout<<"\n";
        }
    }
};
