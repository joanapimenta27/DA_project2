//
// Created by joana on 14-05-2024.
//

#include "DeliveryManager.h"
#include <chrono>
#include <climits>

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
            int in_orig = deliveryGraph_->findVertex(orig)->getIndegree();
            in_orig++;
            deliveryGraph_->findVertex(orig)->setIndegree(in_orig);
            int in_dest = deliveryGraph_->findVertex(dest)->getIndegree();
            in_dest++;
            deliveryGraph_->findVertex(dest)->setIndegree(in_dest);

            double distance= std::stod(line.at(2));

            deliveryGraph_->addBidirectionalEdge(orig,dest,distance);
        }
    }
}

std::unique_ptr<Graph<int> > &DeliveryManager::getDeliveryGraph() {
    return deliveryGraph_;
}


void DeliveryManager::backtrack_tsp(std::unique_ptr<Graph<int>>& g,int vis, Vertex<int>* v,double& res,double cost) {
    if(vis==g->getVertexSet().size()) {
        for(Edge<int>* e: v->getAdj()) {
            if(e->getDest()->getInfo()==0) {
                res=std::min(res,e->getWeight()+cost);
            }
        }
        return;

    }
    for(Edge<int>* e: v->getAdj()) {
        Vertex<int>* dest=e->getDest();

        if(!dest->isVisited()) {
            dest->setVisited(true);
            vis++;
            backtrack_tsp(g,vis,dest,res,cost+e->getWeight());
            dest->setVisited(false);
            vis--;

        }
    }
}

std::pair<double, double> DeliveryManager::backtracking(std::unique_ptr<Graph<int>>& g) {
    auto start=std::chrono::high_resolution_clock::now();
    std::ios_base::sync_with_stdio(false);
    double res=INT_MAX;
    for(Vertex<int> * v : g->getVertexSet()) {
        v->setVisited(false);
    }
    g->findVertex(0)->setVisited(true);
    backtrack_tsp(g,1,g->findVertex(0),res,0);
    auto end=std::chrono::high_resolution_clock::now();
    double res_t=std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
    res_t*=1e-9;
    return  std::make_pair(res,res_t);

}


