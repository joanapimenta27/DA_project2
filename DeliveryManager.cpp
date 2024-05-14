//
// Created by joana on 14-05-2024.
//

#include "DeliveryManager.h"

DeliveryManager::DeliveryManager(std::string choice)
{

    Parser nodes("../dataset/");
    Parser edges("../dataset");



    for (std::vector<std::string> line : nodes.getData()){
        deliveryGraph->addVertex(std::stoi(line.at(0)));
        deliveryGraph->findVertex(std::stoi(line.at(0)))->setLongitude(std::stod(line.at(1)));
        deliveryGraph->findVertex(std::stoi(line.at(0)))->setLatitude(std::stod(line.at(2)));
    }

    for (std::vector<std::string> line : edges.getData()){
        int orig =std::stoi(line.at(0));
        int dest =std::stoi(line.at(1));
        double distance= std::stod(line.at(2));

        deliveryGraph->addEdge(orig,dest,distance);

    }
}
