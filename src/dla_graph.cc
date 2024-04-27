#include "dla_graph.hh"

#include <fstream>

namespace DLA {

Graph::Graph() {
    adjacency_list_ = std::vector<std::vector<int>>();
    nodes_list_ = std::vector<std::shared_ptr<Node>>();

    // add a dummy node at the beginning
    adjacency_list_.push_back(std::vector<int>());
    nodes_list_.push_back(nullptr);
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