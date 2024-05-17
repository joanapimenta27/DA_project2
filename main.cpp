#include <iostream>
#include "Interface.h"

int main()
{
    Interface inter;
    inter.run();
    /*
    DeliveryManager delivery_manager("../dataset/Toy-Graphs/tourism.csv","");
    for(auto v:delivery_manager.getDeliveryGraph()->getVertexSet()) {
        for(auto e:delivery_manager.getDeliveryGraph()->findVertex(v->getInfo())->getAdj()) {
            std::cout<<v->getInfo()<<","<<e->getDest()->getInfo()<<","<<e->getWeight()<<"\n";
        }
    }
    std::cout<<delivery_manager.backtracking(delivery_manager.getDeliveryGraph());

    */

    return 0;
}
