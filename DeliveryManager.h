//
// Created by joana on 14-05-2024.
//

#ifndef DELIVERYMANAGER_H
#define DELIVERYMANAGER_H

#include <memory>

#include "Graph.h"
#include "Parser.h"
#include "Haversine.h"

class DeliveryManager{

private:
    std::unique_ptr<Graph<int>> deliveryGraph_  ;


public:
    DeliveryManager(std::string data_choice, std::string edge_choice);

    std::unique_ptr<Graph<int>>& getDeliveryGraph() ;

    std::pair<double,double> backtracking(std::unique_ptr<Graph<int>>& g);

    void backtrack_tsp(std::unique_ptr<Graph<int>>& g,int vis, Vertex<int>* v,double& res,double cost);
};



#endif //DELIVERYMANAGER_H
