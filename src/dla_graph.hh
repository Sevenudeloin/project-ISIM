#pragma once

#include <memory>
#include <ostream>
#include <vector>

namespace DLA {

struct Node
{
    int label_; // identifier starting from 1
    float y_;
    float x_;
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

    /**
     * @brief Construct a new Graph object. Add a dummy node at the beginning.
     */
    Graph();

    /**
     * @brief Check if there are graph nodes in a certain radius around given coordinates.
     *
     * @param[in] y       y given coordinate
     * @param[in] x       x given coordinate
     * @param[in] radius  radius around the given coordinates
     *
     * @return vector of node labels around the given coordinates, empty if none
     */
    std::vector<int> getNodesAround(float y, float x, float radius);

    /**
     * @param[in] edges_to_add  A vector of pairs of integers containing edge labels to add to the graph.
     */
    void addEdges(const std::vector<std::array<int, 2>>& edges_to_add);

    /**
     * @param[in] edges_to_remove  A vector of pairs of integers containing edge labels to remove from the graph.
     */
    void removeEdges(const std::vector<std::array<int, 2>>& edges_to_remove);

    /**
     * @brief Export the graph under the DOT format (.dot).
     * Can be exported to png using graphviz: `dot -Tpng graph.dot -o graph.png`
     *
     * @param[in] filename  The name of the file to export the graph to.      
     */
    void exportToDot(const std::string& filename);

    /**
     * @brief Export the height of each graph node to a file.
     *
     * @param[in] filename  The name of the file to export the nodes' height to.
     */
    void exportNodesHeight(const std::string& filename);
};

} // namespace DLA