//
// Created by joana on 14-05-2024.
//

#include "DeliveryManager.h"

DeliveryManager::DeliveryManager(std::string vertex_file, std::string edge_file):deliveryGraph_(std::make_unique<Graph<int>>())
{
    if(edge_file=="") {
        Parser edges(vertex_file);

        for (std::vector<std::string> line : edges.getData()){
            if(deliveryGraph_->findVertex(std::stoi(line.at(0)))==nullptr){
                deliveryGraph_->addVertex(std::stoi(line.at(0)));
            }
            if(deliveryGraph_->findVertex(std::stoi(line.at(1)))==nullptr){
                deliveryGraph_->addVertex(std::stoi(line.at(1)));
            }
            int orig =std::stoi(line.at(0));
            int dest =std::stoi(line.at(1));
            double distance= std::stod(line.at(2));

            deliveryGraph_->addBidirectionalEdge(orig,dest,distance);
        }

    }
    else {
        Parser nodes(vertex_file);
        Parser edges(edge_file);


        for (std::vector<std::string> line : nodes.getData()){
            deliveryGraph_->addVertex(std::stoi(line.at(0)));
            deliveryGraph_->findVertex(std::stoi(line.at(0)))->setLongitude(std::stod(line.at(1)));
            deliveryGraph_->findVertex(std::stoi(line.at(0)))->setLatitude(std::stod(line.at(2)));
        }

        for (std::vector<std::string> line : edges.getData()){
            int orig =std::stoi(line.at(0));
            int dest =std::stoi(line.at(1));
            double distance= std::stod(line.at(2));

            deliveryGraph_->addBidirectionalEdge(orig,dest,distance);

        }
    }
}

const std::unique_ptr<Graph<int>> &DeliveryManager::getDeliveryGraph() const{
    return deliveryGraph_;
}
