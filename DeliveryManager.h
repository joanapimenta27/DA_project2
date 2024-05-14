//
// Created by joana on 14-05-2024.
//

#ifndef DELIVERYMANAGER_H
#define DELIVERYMANAGER_H

#include <memory>

#include "Graph.h"
#include "Parser.h"

class DeliveryManager{

private:
    std::unique_ptr<Graph<int>> deliveryGraph_;


public:
    DeliveryManager(std::string data_choice, std::string edge_choice);

    const std::unique_ptr<Graph<int>> &getDeliveryGraph() const;
};



#endif //DELIVERYMANAGER_H
