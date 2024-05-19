//
// Created by joana on 14-05-2024.
//

#include "DeliveryManager.h"
#include <chrono>
#include <climits>
#include <stack>


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




/*In this approach, you are asked to implement another heuristic of your choice to solve the TSP again
starting and ending the node tour on node with the zero-identifier label. The emphasis in this approach
should be on efficiency as this algorithm should be feasible even for the large graphs. Here you can make
use of several algorithmic techniques from the use of divide-and-conquer to splitting the graph into multiple
geographic sections to the use of clustering, or both, possibly even in combination with the 2-
approximation algorithm. In the end, you are strongly suggested to compare the quality and run-time
performance of your heuristic solution against the 2-approximation algorithm. You are expected to present
only one heuristic that works well over the basic Triangular approximation or one that offers comparable
tour length results but with noticeably faster execution time (i.e., time complexity). You should avoid
investing a tremendous effort in developing an heuristic that yields marginal gains. It is also important to
demonstrate what gains you achieved with your heuristic, so, during the presentation, a comparative analysis
between the backtracking, the triangular approximation and your algorithm should be shown.
Once again, in order to directly compare your results with the triangular approximation heuristic, you should
assume that all given graphs are fully connected and use the Haversine distance to compute the weights
of the edges that are not explicitly described in the dataset, thus making the graph fully connected.
 */

// Created by joana on 14-05-2024.

// Find odd degree vertices in MST
std::vector<Vertex<int>*> DeliveryManager::findOddDegreeVertices(std::unique_ptr<Graph<int>>& g, const std::vector<int>& mst) {
    std::unordered_map<int, int> degreeCount;
    for(int i = 0; i < mst.size(); ++i) {
        if(mst[i] != -1) {
            degreeCount[i]++;
            degreeCount[mst[i]]++;
        }
    }
    std::vector<Vertex<int>*> oddVertices;
    for(auto& pair : degreeCount) {
        if(pair.second % 2 == 1) {
            oddVertices.push_back(g->findVertex(pair.first));
        }
    }
    return oddVertices;
}

// Minimum Weight Perfect Matching using a naive approach (for simplicity)
std::vector<std::pair<Vertex<int>*, Vertex<int>*>> DeliveryManager::minimumWeightPerfectMatching(std::vector<Vertex<int>*>& oddVertices) {
    std::vector<std::pair<Vertex<int>*, Vertex<int>*>> matching;
    for(int i = 0; i < oddVertices.size(); ++i) {
        double minDistance = std::numeric_limits<double>::max();
        Vertex<int>* closestVertex = nullptr;
        for(int j = 0; j < oddVertices.size(); ++j) {
            if(i != j) {
                double distance = calculate_distance(oddVertices[i]->getLatitude(), oddVertices[i]->getLongitude(), oddVertices[j]->getLatitude(), oddVertices[j]->getLongitude());
                if(distance < minDistance) {
                    minDistance = distance;
                    closestVertex = oddVertices[j];
                }
            }
        }
        matching.push_back({oddVertices[i], closestVertex});
    }
    return matching;
}

// Combine MST and MWPM to form Eulerian graph
std::vector<Edge<int>*> DeliveryManager::combineMSTandMWPM(const std::vector<Edge<int>*>& mstEdges, const std::vector<Edge<int>*>& mwpmEdges) {
    std::vector<Edge<int>*> eulerianEdges;
    for(auto& edge : mstEdges) {
        eulerianEdges.push_back(edge);
    }
    for(auto& edge : mwpmEdges) {
        eulerianEdges.push_back(edge);
    }
    return eulerianEdges;
}

// Eulerian Circuit using Hierholzer's Algorithm
std::vector<int> DeliveryManager::eulerianCircuit(std::unique_ptr<Graph<int>>& g, const std::vector<Edge<int>*>& eulerianEdges) {
    std::unordered_map<int, std::vector<int>> adjList;
    for(auto& edge : eulerianEdges) {
        adjList[edge->getOrig()->getInfo()].push_back(edge->getDest()->getInfo());
    }
    std::vector<int> circuit;
    std::stack<int> stack;
    int currentVertex = 0;
    stack.push(currentVertex);
    while(!stack.empty()) {
        if(adjList[currentVertex].empty()) {
            circuit.push_back(currentVertex);
            currentVertex = stack.top();
            stack.pop();
        } else {
            stack.push(currentVertex);
            int nextVertex = adjList[currentVertex].back();
            adjList[currentVertex].pop_back();
            currentVertex = nextVertex;
        }
    }
    return circuit;
}

double DeliveryManager::calculateTSPPath(const std::vector<int>& eulerianCircuit, std::unique_ptr<Graph<int>>& g) {
    std::vector<bool> visited(eulerianCircuit.size(), false);
    double cost = 0.0;
    int prev = eulerianCircuit[0];
    visited[prev] = true;

    for(size_t i = 1; i < eulerianCircuit.size(); ++i) {
        int current = eulerianCircuit[i];
        if(!visited[current]) {
            visited[current] = true;
            cost += calculate_distance(g->findVertex(prev)->getLatitude(), g->findVertex(prev)->getLongitude(),
                                       g->findVertex(current)->getLatitude(), g->findVertex(current)->getLongitude());
            prev = current;
        }
    }
    cost += calculate_distance(g->findVertex(prev)->getLatitude(), g->findVertex(prev)->getLongitude(),
                               g->findVertex(0)->getLatitude(), g->findVertex(0)->getLongitude());
    return cost;
}

// Heuristic approach to solve TSP
double DeliveryManager::heuristicTSP(std::unique_ptr<Graph<int>>& g) {
    std::vector<int> mstPredecessors = mstPrim(g);

    std::vector<Edge<int>*> mstEdges;
    for(size_t i = 0; i < mstPredecessors.size(); ++i) {
        if(mstPredecessors[i] != -1) {
            mstEdges.push_back(new Edge<int>(g->findVertex(i), g->findVertex(mstPredecessors[i]),
                                             calculate_distance(g->findVertex(i)->getLatitude(), g->findVertex(i)->getLongitude(),
                                                                g->findVertex(mstPredecessors[i])->getLatitude(), g->findVertex(mstPredecessors[i])->getLongitude())));
        }
    }

    std::vector<Vertex<int>*> oddVertices = findOddDegreeVertices(g, mstPredecessors);
    std::vector<std::pair<Vertex<int>*, Vertex<int>*>> mwpmVertices = minimumWeightPerfectMatching(oddVertices);
    std::vector<Edge<int>*> mwpmEdges;
    for(auto& pair : mwpmVertices) {
        mwpmEdges.push_back(new Edge<int>(pair.first, pair.second,
                                          calculate_distance(pair.first->getLatitude(), pair.first->getLongitude(),
                                                             pair.second->getLatitude(), pair.second->getLongitude())));
    }

    std::vector<Edge<int>*> eulerianEdges = combineMSTandMWPM(mstEdges, mwpmEdges);
    std::vector<int> eulerianCircuit1 = eulerianCircuit(g, eulerianEdges);
    return calculateTSPPath(eulerianCircuit1, g);
}


