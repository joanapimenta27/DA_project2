//
// Created by joana on 14-05-2024.
//

#ifndef DELIVERYMANAGER_H
#define DELIVERYMANAGER_H

#include <memory>
#include <set>

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

    double tsp2Approximation(std::unique_ptr<Graph<int>>& g);

    std::vector<int> mstPrim(std::unique_ptr<Graph<int>>& g);

    std::set<int> transformPrim(std::vector<std::vector<int>> v );

    void dfsPrim(std::unique_ptr<Graph<int>>& g,int v,std::vector<int>& res,std::vector<int> prim);

    void updateQueue( std::priority_queue<std::pair<double, Vertex<int>*>, std::vector<std::pair<double, Vertex<int>*>>, std::greater<std::pair<double, Vertex<int>*>>>& V,double w, Vertex<int>* v);

    Edge<int> * findEdge(std::unique_ptr<Graph<int>>& g,const int &in,const int &dest);

    std::vector<Vertex<int> *> findOddDegreeVertices(std::unique_ptr<Graph<int>> &g, const std::vector<int> &mst);

    std::vector<std::pair<Vertex<int>*, Vertex<int>*>> minimumWeightPerfectMatching(std::vector<Vertex<int>*>& oddVertices);

    std::vector<Edge<int> *> combineMSTandMWPM(const std::vector<Edge<int> *> &mstEdges, const std::vector<Edge<int> *> &mwpmEdges);

    double calculateTSPPath(const std::vector<int> &eulerianCircuit, std::unique_ptr<Graph<int>> &g);

    double heuristicTSP(std::unique_ptr<Graph<int>> &g);

    std::vector<int> eulerianCircuit(std::unique_ptr<Graph<int>> &g, const std::vector<Edge<int> *> &eulerianEdges);
};



#endif //DELIVERYMANAGER_H
