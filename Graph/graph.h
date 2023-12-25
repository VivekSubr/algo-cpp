#pragma once
#include <vector>
#include <unordered_map>
#include <tuple>
#include <iostream>
#include <memory>
#include <string>

enum class GraphRepresentation 
{
    AdjMatrix = 0,
    AdjList
};

class Graph 
{
public:
    Graph() = default;
    Graph(int nodes, bool directed = false) {} 
    Graph(int nodes, bool directed, const std::vector<std::tuple<int, int, int>>& edges) {}

    virtual ~Graph() {}
 
    virtual void addEdge(int i, int j, int weight=1) = 0;
    virtual void addEdges(const std::vector<std::tuple<int, int, int>>& edges) = 0;
    virtual bool hasEdge(int i, int j) const = 0;
    virtual int  getEdgeWeight(int i, int j) const = 0;
    virtual void deleteNode(int node)   = 0;
    virtual int  getNumberNodes() const = 0;
    virtual int  inDegree(int node)     = 0;
    virtual int  outDegree(int node)    = 0;
    virtual bool isDirected() const     = 0;
    virtual std::vector<int> getNeighbors(int node) = 0;
    virtual std::string      getName() const = 0;
    virtual std::vector<std::tuple<int, int, int>> getEdges() = 0;
};

class GraphMatrix : public Graph
{
public:
    GraphMatrix(int nodes, bool directed = false): m_nNodes(nodes), m_isDirected(directed)
    {
        m_name = "AdjMatrix" + std::to_string(nodes) + (directed ? "Directed" : "unDirected");
        m_adjMatrix.resize(m_nNodes, std::vector<int>(m_nNodes, 0));
    }

    GraphMatrix(int nodes, bool directed, const std::vector<std::tuple<int, int, int>>& edges): m_nNodes(nodes), m_isDirected(directed)
    {
        m_name = "AdjMatrix" + std::to_string(nodes) + (directed ? "Directed" : "unDirected");
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

    int getNumberNodes() const  { return m_nNodes; }
    std::string getName() const { return m_name;   }
    bool isDirected() const     { return m_isDirected; }

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

    std::vector<int> getNeighbors(int node)
    {
        std::vector<int> ret;
        for(int i=0; i<m_nNodes; i++)
        {
            if(i != node && hasEdge(node, i))
            {
                ret.push_back(i);
            }
        }

        return ret;
    }

    std::vector<std::tuple<int, int, int>> getEdges()
    {
        std::vector<std::tuple<int, int, int>> ret;

        for(int i=0; i<m_nNodes; i++)
        {
            for(int j=0; j<m_nNodes; j++)
            {
                if(m_adjMatrix[i][j]) 
                {
                    ret.push_back({i, j, m_adjMatrix[i][j]});
                }
            }
        }

        return ret;
    }

private:
    int                           m_nNodes;
    bool                          m_isDirected;
    std::string                   m_name;
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

class GraphList : public Graph
{
public:
    GraphList(int nodes, bool directed = false): m_nNodes(nodes), m_isDirected(directed)
    {
        m_name = "AdjList" + std::to_string(nodes) + (directed ? "Directed" : "unDirected");
    }

    GraphList(int nodes, bool directed, const std::vector<std::tuple<int, int, int>>& edges): m_nNodes(nodes), m_isDirected(directed)
    {
        m_name = "AdjList" + std::to_string(nodes) + (directed ? "Directed" : "unDirected");
        addEdges(edges);
    }

    void addEdge(int i, int j, int weight=1)
    {
        m_AdjMap[i].insert({j, weight});
        if(!m_isDirected) {
            m_AdjMap[j].insert({i, weight});
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
        auto nodeMap = m_AdjMap.find(i);
        if(nodeMap != m_AdjMap.end() && nodeMap->second.find(j) != nodeMap->second.end())
        {
            return true;
        }

        return false;
    }

    int getEdgeWeight(int i, int j) const
    {
        auto nodeMap = m_AdjMap.find(i);
        if(nodeMap != m_AdjMap.end())
        {
            auto edge = nodeMap->second.find(j);
            if(edge != nodeMap->second.end())
            {
               return edge->second;
            }
        }

        return 0;
    }

    void deleteNode(int node)
    {
        m_AdjMap.erase(node);
    }

    int inDegree(int node) 
    {
        if(!m_isDirected) return -1;
        
        int degree = 0;
        for(const auto& nodeMap : m_AdjMap)
        {
            if(nodeMap.second.find(node) != nodeMap.second.end()) ++degree;
        }

        return degree;
    }

    int outDegree(int node) 
    {
        if(!m_isDirected) return -1;

        auto nodeMap = m_AdjMap.find(node);
        if(nodeMap->second.find(node) != nodeMap->second.end())
        {
            return nodeMap->second.size();
        }

        return 0;
    }

    std::vector<int> getNeighbors(int node)
    {
        std::vector<int> ret;
        auto nodeMap = m_AdjMap.find(node);
        if(nodeMap != m_AdjMap.end())
        {
            for(const auto& neighbor : nodeMap->second)
            {
                ret.push_back(neighbor.first);
            }
        }

        return ret;
    }

    int getNumberNodes() const  { return m_nNodes; }
    std::string getName() const { return m_name;   }
    bool isDirected() const     { return m_isDirected; }

    std::vector<std::tuple<int, int, int>> getEdges()
    {
        std::vector<std::tuple<int, int, int>> ret;

        for(int node=0; node<m_nNodes; node++)
        {
            auto nodeMap = m_AdjMap.find(node);
            if(nodeMap != m_AdjMap.end())
            {
                for(const auto& neighbor : nodeMap->second)
                {
                    ret.push_back({ node, neighbor.first, neighbor.second});
                }
            }
        }

        return ret;
    }

private:
    int         m_nNodes;
    bool        m_isDirected;
    std::string m_name;

    //technically adjacency map, each node has map of connected node : cost 
    std::unordered_map<int, std::unordered_map<int, int>> m_AdjMap;
};

std::shared_ptr<Graph> CreateGraph(int nodes, bool directed, GraphRepresentation type)
{
    switch(type)
    {
        case GraphRepresentation::AdjMatrix:
            return std::make_shared<GraphMatrix>(nodes, directed);

        case GraphRepresentation::AdjList:
            return std::make_shared<GraphList>(nodes, directed);
    }

    return nullptr;
}