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

void Graph::exportToDot(const std::string &filename) { // FIXME CHECK FUNCTION !!! NOT VERIFIED FIXME FIXME
    std::ofstream file(filename);

    file << "graph DLA {" << std::endl;

    for (int i = 1; i < nodes_list_.size(); i++) {
        file << "    " << nodes_list_[i]->label_ << " [label=\"" << nodes_list_[i]->label_ << "\"];" << std::endl;
    }

    for (int i = 1; i < adjacency_list_.size(); i++) {
        for (int j = 0; j < adjacency_list_[i].size(); j++) {
            file << "    " << nodes_list_[i]->label_ << " -- " << nodes_list_[adjacency_list_[i][j]]->label_ << ";" << std::endl;
        }
    }

    file << "}" << std::endl;
    file.close();
}

}