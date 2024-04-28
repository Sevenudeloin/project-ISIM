#include "dla_graph.hh"

#include <algorithm>
#include <fstream>

namespace DLA {

Graph::Graph() {
    adjacency_list_ = std::vector<std::vector<int>>();
    nodes_list_ = std::vector<std::shared_ptr<Node>>();

    // add a dummy node at the beginning
    adjacency_list_.push_back(std::vector<int>());
    nodes_list_.push_back(nullptr);
}

void Graph::addEdges(const std::vector<std::array<int, 2>>& edges_to_add) {
    for (const auto& edge : edges_to_add) {
        int node1_label = edge[0];
        int node2_label = edge[1];

        // add the edge to the adjacency list
        adjacency_list_[node1_label].push_back(node2_label);
        adjacency_list_[node2_label].push_back(node1_label);
    }
}

void Graph::removeEdges(const std::vector<std::array<int, 2>>& edges_to_remove) {
    for (const auto& edge : edges_to_remove) {
        int node1_label = edge[0];
        int node2_label = edge[1];

        // erase-remove idiom
        adjacency_list_[node1_label].erase(std::remove(adjacency_list_[node1_label].begin(), adjacency_list_[node1_label].end(), node2_label), adjacency_list_[node1_label].end());
        adjacency_list_[node2_label].erase(std::remove(adjacency_list_[node2_label].begin(), adjacency_list_[node2_label].end(), node1_label), adjacency_list_[node2_label].end());
    } 
}

void Graph::exportToDot(const std::string& filename) { // FIXME CHECK FUNCTION !!! NOT VERIFIED FIXME FIXME
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

}