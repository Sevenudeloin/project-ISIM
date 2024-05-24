#pragma once

#include <memory>
#include <ostream>
#include <vector>

namespace DLA {

struct Node
{
    int label_; // identifier starting from 1
    int y_;
    int x_;
    float height_;

};

inline std::ostream& operator<<(std::ostream& os, const Node& node)
{
    os << "Node " << node.label_ << " (y: " << node.y_ << ", x: " << node.x_ << ") - height: " << node.height_ << std::endl;
    return os;
}

struct Graph
{
    using AdjacencyList = std::vector<std::vector<int>>; // adjacency list representation of the graph (by label, position in the list is the label of the node)
    using NodesList = std::vector<std::shared_ptr<Node>>; // list of nodes in the graph (position of the node in the vector should be the label of the node)

    AdjacencyList adjacency_list_;
    NodesList nodes_list_;

    Graph();

    void addEdges(const std::vector<std::array<int, 2>>& edges_to_add);
    void removeEdges(const std::vector<std::array<int, 2>>& edges_to_remove);

    void exportToDot(const std::string& filename);
    void exportNodesHeight(const std::string& filename);
};

} // namespace DLA