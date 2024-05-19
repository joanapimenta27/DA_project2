#include <iostream>
#include "Interface.h"

int main()
{   DeliveryManager delivery_manager("../dataset/Toy-Graphs/stadiums.csv","");
    std::cout<<delivery_manager.tsp2Approximation(delivery_manager.getDeliveryGraph());
    /*auto e=delivery_manager.tsp2Approximation(delivery_manager.getDeliveryGraph());
    for(auto a:e) {
        for(auto m:a) {
            std::cout<<m<<" , ";
        }
        std::cout<<" | ";
    }


    Interface inter;
    inter.run();


    DeliveryManager delivery_manager("../dataset/Toy-Graphs/tourism.csv","");
    std::priority_queue<int,std::vector<int>,std::greater<int>> V;

    for(auto v:delivery_manager.getDeliveryGraph()->getVertexSet()) {
        for(auto e:delivery_manager.getDeliveryGraph()->findVertex(v->getInfo())->getAdj()) {
            std::cout<<v->getInfo()<<","<<e->getDest()->getInfo()<<","<<e->getWeight()<<"\n";
        }
    }
    */




    return 0;
}
