#include "dla_graph.hh"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <string>

#include "utils.hh"

namespace DLA {

/**
 * @brief Construct a new Graph object. Add a dummy node at the beginning (nullptr in node list and empty list in adjacency list).
 */
Graph::Graph() {
    adjacency_list_ = std::vector<std::vector<int>>();
    nodes_list_ = std::vector<std::shared_ptr<Node>>();

    adjacency_list_.push_back(std::vector<int>());
    nodes_list_.push_back(nullptr);
}

// float euclidianDistance(float y1, float x1, float y2, float x2) {
//     return std::sqrt(std::pow(y1 - y2, 2) + std::pow(x1 - x2, 2));
// }

/**
 * @brief Check if there are graph nodes in a certain radius around given coordinates
 *
 * @param[in] y       y given coordinate
 * @param[in] x       x given coordinate
 * @param[in] radius  radius around the given coordinates
 *
 * @return vector of node labels around the given coordinates, empty if none
 */
std::vector<int> Graph::getNodesAround(float y, float x, float radius) {
    std::vector<int> labels_around = {};

    for (size_t i = 1; i < nodes_list_.size(); i++) {
        float node_y = nodes_list_[i]->y_;
        float node_x = nodes_list_[i]->x_;

        if (utils::euclidianDistance(y, x, node_y, node_x) <= radius) {
            labels_around.push_back(i);
        }
    }

    return labels_around;
}

/**
 * @brief Graph is undirected, so we add the edge to both nodes' adjacency list.
 *
 * @param edges_to_add  A vector of pairs of integers containing edges to add to the graph.
 */
void Graph::addEdges(const std::vector<std::array<int, 2>>& edges_to_add) {
    for (const auto& edge : edges_to_add) {
        int node1_label = edge[0];
        int node2_label = edge[1];

        // add the edge to the adjacency list
        adjacency_list_[node1_label].push_back(node2_label);
        adjacency_list_[node2_label].push_back(node1_label);
    }
}

/**
 * @brief Graph is undirected, so we remove the edge from both nodes' adjacency list.
 *
 * @param edges_to_remove  A vector of pairs of integers containing edges to remove from the graph.
 */
void Graph::removeEdges(const std::vector<std::array<int, 2>>& edges_to_remove) {
    for (const auto& edge : edges_to_remove) {
        int node1_label = edge[0];
        int node2_label = edge[1];

        // erase-remove idiom
        adjacency_list_[node1_label].erase(std::remove(adjacency_list_[node1_label].begin(), adjacency_list_[node1_label].end(), node2_label), adjacency_list_[node1_label].end());
        adjacency_list_[node2_label].erase(std::remove(adjacency_list_[node2_label].begin(), adjacency_list_[node2_label].end(), node1_label), adjacency_list_[node2_label].end());
    } 
}

/**
 * @brief Export the graph under the DOT format (.dot).
 * Can be exported to png using graphviz: `dot -Tpng graph.dot -o graph.png`
 *
 * @param filename  The name of the file to export the graph to.      
 */
void Graph::exportToDot(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("DLA Graph: exportToDot: Could not open file " + filename);
    }

    file << "graph DLA {" << std::endl;

    for (size_t i = 1; i < adjacency_list_.size(); i++) {
        for (size_t j = 0; j < adjacency_list_[i].size(); j++) {
            file << "    " << nodes_list_[i]->label_ << " -- " << nodes_list_[adjacency_list_[i][j]]->label_ << ";" << std::endl;
        }
    }

    file << "}" << std::endl;
    file.close();
}

void Graph::exportNodesHeight(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("DLA Graph: exportNodesHeight: Could not open file " + filename);
    }

    for (size_t i = 1; i < nodes_list_.size(); i++) {
        file << "Node " << i << ": " << nodes_list_[i]->height_ << std::endl;
    }

    file.close();
}

} // namespace DLA