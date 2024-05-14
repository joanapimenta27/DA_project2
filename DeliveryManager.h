//
// Created by joana on 14-05-2024.
//

#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <memory>

#include "Graph.h"
#include "Parser.h"

class DeliveryManager{

private:
    std::unique_ptr<Graph<int>> deliveryGraph;


public:
    DeliveryManager(std::string choice);
};



#endif //FILEPARSER_H
