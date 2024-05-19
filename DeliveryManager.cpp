//
// Created by joana on 14-05-2024.
//

#include "DeliveryManager.h"
#include <chrono>
#include <climits>
#include <set>

#include "Haversine.h"


DeliveryManager::DeliveryManager(std::string vertex_file, std::string edge_file):deliveryGraph_(std::make_unique<Graph<int>>()),vertex_(std::unordered_map<int, Vertex<int>*,vert_struct>())
{
    if(edge_file=="") {
        Parser edges(vertex_file);

        for (std::vector<std::string> line : edges.getData()){
            if(vertex_.find(std::stoi(line.at(0)))==nullptr){
                vertex_.insert({std::stoi(line.at(0)),new Vertex<int>(std::stoi(line.at(0)))});
                deliveryGraph_->vertexSet.push_back(vertex_[std::stoi(line.at(0))]);

            }
            if(vertex_.find(std::stoi(line.at(1)))==nullptr){
                vertex_.insert({std::stoi(line.at(1)),new Vertex<int>(std::stoi(line.at(1)))});
                deliveryGraph_->vertexSet.push_back(vertex_[std::stoi(line.at(1))]);

            }
            int orig =std::stoi(line.at(0));
            int dest =std::stoi(line.at(1));
            double distance= std::stod(line.at(2));

            vertex_[orig]->addEdge(vertex_[dest],distance);
            vertex_[dest]->addEdge(vertex_[orig],distance);
        }

    }
    else {
        Parser nodes(vertex_file);
        Parser edges(edge_file);


        for (std::vector<std::string> line : nodes.getData()){
            vertex_.insert({std::stoi(line.at(0)),new Vertex<int>(std::stoi(line.at(0)))});
            deliveryGraph_->vertexSet.push_back(vertex_[std::stoi(line.at(0))]);
            vertex_[std::stoi(line.at(0))]->setLongitude(std::stod(line.at(1)));
            vertex_[std::stoi(line.at(0))]->setLatitude(std::stod(line.at(2)));
        }

        for (std::vector<std::string> line : edges.getData()){
            int orig =std::stoi(line.at(0));
            int dest =std::stoi(line.at(1));

            double distance= std::stod(line.at(2));

            vertex_[orig]->addEdge(vertex_[dest],distance);
            vertex_[dest]->addEdge(vertex_[orig],distance);
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
        double newcost=cost+e->getWeight();
        if(!dest->isVisited()) {
            if(newcost<res) {
                dest->setVisited(true);
                vis++;
                backtrack_tsp(g,vis,dest,res,newcost);
                dest->setVisited(false);
                vis--;
            }
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


std::vector<int> DeliveryManager::mstPrim(std::unique_ptr<Graph<int>>& g) {
    std::vector<int> pre(g->getVertexSet().size(), -1);
    std::vector<double> key(g->getVertexSet().size(), INT_MAX);
    std::priority_queue<std::pair<double, Vertex<int>*>, std::vector<std::pair<double, Vertex<int>*>>, std::greater<std::pair<double, Vertex<int>*>>> V;

    for(Vertex<int>* v :g->getVertexSet()) {
        if(v->getInfo()==0) {
            V.emplace(0,v);
            key[0]=0;
            v->setVisited(true);
        }
        else {
            V.emplace(INT_MAX,v);
            v->setVisited(false);
        }

    }

    while(!V.empty()) {
        std::pair<double,Vertex<int>*> tp=V.top();
        V.pop();
        Vertex<int>* u=tp.second;
        u->setVisited(true);

        for(Edge<int>* e:u->getAdj()) {
            Vertex<int>* dest=e->getDest();
            double w=e->getWeight();
            if(!dest->isVisited()&& w<key[dest->getInfo()]) {
                pre[dest->getInfo()]=u->getInfo();
                key[dest->getInfo()]=w;
                updateQueue(V,w,dest);
            }
        }
    }
    return pre;
}

void DeliveryManager::updateQueue( std::priority_queue<std::pair<double, Vertex<int>*>, std::vector<std::pair<double, Vertex<int>*>>, std::greater<std::pair<double, Vertex<int>*>>>& V,double w,Vertex<int>* v) {
    std::vector<std::pair<double,Vertex<int>*>> tmp;
    while(!V.empty()) {
       std::pair<double,Vertex<int>*> tp=V.top();
        if(tp.second==v) {
            tp.first=w;
        }
        V.pop();
        tmp.push_back(tp);
    }

    for(std::pair<double,Vertex<int>*>& t:tmp) {
        V.push(t);
    }
}

void DeliveryManager::dfsPrim(std::unique_ptr<Graph<int>>& g,int v,std::vector<int>& res,std::vector<int> prim) {
    if(!g->findVertex(v)->isVisited()) {
        g->findVertex(v)->setVisited(true);
        bool flag=true;
        res.push_back(v);
        for(int i=0;i<prim.size();i++) {
            if(prim[i]==v && !g->findVertex(i)->isVisited()) {
                dfsPrim(g,i,res,prim);
                flag=false;
            }
        }



    }
}

Edge<int> * DeliveryManager::findEdge(std::unique_ptr<Graph<int>>& g,const int &in,const int &dest){
    for (auto v : g->getVertexSet())
        if (v->getInfo() == in)
            for(auto e:v->getAdj()) {
                if(e->getDest()->getInfo()==dest) {
                    return e;
                }
            }
    return nullptr;
}



std::pair<double,double> DeliveryManager::tsp2Approximation(std::unique_ptr<Graph<int>>& g) {
        double cost=0;
        auto start=std::chrono::high_resolution_clock::now();
        std::ios_base::sync_with_stdio(false);
        std::vector<int> res;
        std::vector<std::vector<int>> ans;
        std::vector<int> pri =mstPrim(g);
        for(Vertex<int>* v:g->getVertexSet()) {
            v->setVisited(false);
        }

        for(int i=0;i<pri.size();i++) {
            dfsPrim(g,i,res,pri);
        }
        res.push_back(0);
        int prev=res[0];
        int aft;
        for(int i=1 ;i!=res.size();i++){
            aft=res[i];
            if(findEdge(g, prev,aft)!=nullptr) {
                double w=findEdge(g,prev,aft)->getWeight();
                cost+=w;
                prev=aft;
            }
            else {
                double w=calculate_distance(g->findVertex(prev)->getLatitude(),g->findVertex(prev)->getLongitude(),g->findVertex(aft)->getLatitude(),g->findVertex(aft)->getLongitude());
                g->addBidirectionalEdge(prev,aft,w);
                cost+=w;
                prev=aft;
            }
        }
    auto end=std::chrono::high_resolution_clock::now();
    double res_t=std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
    res_t*=1e-9;
    return  std::make_pair(cost,res_t);

}








