//
// Created by joana on 14-05-2024.
//

#include "DeliveryManager.h"
#include <chrono>
#include <climits>
#include <stack>


/**
 * @brief Constructor for DeliveryManager class.
 *
 * @details Initializes the delivery graph from the given vertex and edge files.
 *
 * @param vertex_file The file containing the vertex data.
 * @param edge_file The file containing the edge data. If empty, the vertex file contains both vertices and edges.
 * @complexity O(V + E) where V is the number of vertices and E is the number of edges.
 */
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


/**
 * @brief Get the delivery graph.
 *
 * @return A reference to the unique pointer of the delivery graph.
 * @complexity O(1)
 */
std::unique_ptr<Graph<int> > &DeliveryManager::getDeliveryGraph() {
    return deliveryGraph_;
}


/**
 * @brief Backtracking function for solving the Traveling Salesman Problem (TSP).
 *
 * @param g The graph.
 * @param vis The number of visited vertices.
 * @param v The current vertex.
 * @param res The minimum cost result.
 * @param cost The current cost.
 * @complexity O(V!)
 */
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
/**
 * @brief Solve the TSP using backtracking.
 *
 * @param g The graph.
 * @return A pair containing the minimum cost and the time taken to find the solution.
 * @complexity O(V!)
 */
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
/**
 * @brief Find the Minimum Spanning Tree (MST) using Prim's algorithm.
 *
 * @param g The graph.
 * @param start The starting vertex.
 * @return A vector of predecessors in the MST.
 * @complexity O(E log V)
 */
std::vector<int> DeliveryManager::mstPrim(std::unique_ptr<Graph<int>>& g,int start) {
    std::vector<int> pre(g->getVertexSet().size(), -1);
    std::vector<double> key(g->getVertexSet().size(), INT_MAX);
    std::priority_queue<std::pair<double, Vertex<int>*>, std::vector<std::pair<double, Vertex<int>*>>, std::greater<std::pair<double, Vertex<int>*>>> V;

    for(Vertex<int>* v :g->getVertexSet()) {
        if(v->getInfo()==start) {
            V.emplace(start,v);
            key[start]=0;
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
/**
 * @brief Update the priority queue.
 *
 * @param V The priority queue.
 * @param w The weight.
 * @param v The vertex.
 * @complexity O(V)
 */
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

/**
 * @brief Depth-first search used in Prim's algorithm.
 *
 * @param g The graph.
 * @param v The current vertex.
 * @param res The result vector.
 * @param prim The MST predecessor vector.
 * @complexity O(V + E)
 */
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
/**
 * @brief Find the edge between two vertices.
 *
 * @param g The graph.
 * @param in The source vertex.
 * @param dest The destination vertex.
 * @return A pointer to the edge, or nullptr if not found.
 * @complexity O(E)
 */
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


/**
 * @brief 2-Approximation algorithm for solving the Traveling Salesman Problem (TSP).
 *
 * @details This method uses a Minimum Spanning Tree (MST) followed by a depth-first search (DFS)
 * to find a Hamiltonian circuit that approximates the TSP tour within a factor of 2.
 *
 * @param g The graph representing the delivery network.
 * @return A pair containing the total cost of the approximate TSP tour and the time taken to compute it.
 * @complexity O(V^2 log V)
 */
std::pair<double,double> DeliveryManager::tsp2Approximation(std::unique_ptr<Graph<int>>& g) {
        double cost=0;
        auto start=std::chrono::high_resolution_clock::now();
        std::ios_base::sync_with_stdio(false);
        std::vector<int> res;
        std::vector<std::vector<int>> ans;
        std::vector<int> pri =mstPrim(g,0);
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

// Created by joana on 14-05-2024.

// Find odd degree vertices in MST
/**
 * @brief Find odd degree vertices in a Minimum Spanning Tree (MST).
 *
 * @param g The graph representing the delivery network.
 * @param mst The predecessor list representing the MST.
 * @return A vector of vertices with odd degrees.
 * @complexity O(V + E)
 */
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
/**
 * @brief Find the Minimum Weight Perfect Matching (MWPM) for odd degree vertices.
 *
 * @details This naive approach finds the MWPM by checking each pair of vertices and choosing
 * the minimum weight edge connecting them.
 *
 * @param oddVertices The vertices with odd degrees.
 * @return A vector of pairs representing the matched vertices and their connecting edges.
 * @complexity O(V^2)
 */
std::vector<std::pair<Vertex<int>*, Edge<int>*>> DeliveryManager::minimumWeightPerfectMatching(std::vector<Vertex<int>*>& oddVertices) {
    std::vector<std::pair<Vertex<int>*, Edge<int>*>> matching;
    for(int i = 0; i < oddVertices.size(); ++i) {
        double minDistance = INT_MAX;
        Edge<int>* edge = nullptr;
        Edge<int>* closestEdge = nullptr;

        for(int j = 0; j < oddVertices.size(); ++j) {
            if(i != j) {
                Vertex<int>* orig=oddVertices[i];
                Vertex<int>* dest=oddVertices[j];
                double distance=INT_MAX;
                bool flag=true;
                for(auto e:orig->getAdj()) {
                    if(e->getDest()==dest) {
                        distance=e->getWeight();
                        edge=e;
                        flag=false;
                    }

                }
                if(flag) {
                    distance+=calculate_distance(deliveryGraph_->findVertex(orig->getInfo())->getLatitude(),deliveryGraph_->findVertex(orig->getInfo())->getLongitude(),deliveryGraph_->findVertex(dest->getInfo())->getLatitude(),deliveryGraph_->findVertex(dest->getInfo())->getLongitude());

                }

                if(distance < minDistance) {
                    minDistance = distance;
                    closestEdge = edge;
                }
            }
        }
        matching.push_back({oddVertices[i], closestEdge});
    }
    return matching;
}

// Combine MST and MWPM to form Eulerian graph
/**
 * @brief Combine MST edges and MWPM edges to form an Eulerian graph.
 *
 * @param mstEdges The edges of the Minimum Spanning Tree (MST).
 * @param mwpmEdges The edges of the Minimum Weight Perfect Matching (MWPM).
 * @return A vector of edges representing the Eulerian graph.
 * @complexity O(V + E)
 */
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
/**
 * @brief Find an Eulerian Circuit using Hierholzer's Algorithm.
 *
 * @param g The graph representing the delivery network.
 * @param eulerianEdges The edges forming the Eulerian graph.
 * @param start The starting vertex.
 * @return A vector representing the Eulerian circuit.
 * @complexity O(V + E)
 */
std::vector<int> DeliveryManager::eulerianCircuit(std::unique_ptr<Graph<int>>& g, const std::vector<Edge<int>*>& eulerianEdges,int start) {
    std::unordered_map<int, std::vector<int>> adjList;
    for(auto& edge : eulerianEdges) {
        adjList[edge->getOrig()->getInfo()].push_back(edge->getDest()->getInfo());
    }
    std::vector<int> circuit;
    std::stack<int> stack;
    int currentVertex = start;
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

/**
 * @brief Calculate the cost of a TSP path from an Eulerian circuit.
 *
 * @details This method removes repeated vertices to find a Hamiltonian circuit from the Eulerian circuit
 * and calculates the total cost of the TSP path.
 *
 * @param eulerianCircuit The Eulerian circuit.
 * @param g The graph representing the delivery network.
 * @param eulerianEdges The edges forming the Eulerian graph.
 * @return The total cost of the TSP path.
 * @complexity O(V + E)
 */
double DeliveryManager::calculateTSPPath(const std::vector<int>& eulerianCircuit, std::unique_ptr<Graph<int>>& g,const std::vector<Edge<int>*>& eulerianEdges) {
    std::vector<bool> visited(eulerianCircuit.size(), false);
    double cost = 0.0;
    int prev = eulerianCircuit[0];
    visited[prev] = true;
    for(auto e:deliveryGraph_->findVertex(prev)->getAdj()) {
        if(e->getDest()->getInfo()==0) {
            cost+=e->getWeight();
            break;
        }
    }

    for(size_t i = 1; i < eulerianCircuit.size(); ++i) {
        int current = eulerianCircuit[i];
        if(!visited[current]) {
            visited[current] = true;
            bool flag=true;
            for(auto e:eulerianEdges) {
                if(e->getOrig()->getInfo()==prev) {
                    cost+=e->getWeight();
                    flag=false;
                    break;
                }
            }
            if(flag) {
                cost+=calculate_distance(deliveryGraph_->findVertex(prev)->getLatitude(),deliveryGraph_->findVertex(prev)->getLongitude(),deliveryGraph_->findVertex(current)->getLatitude(),deliveryGraph_->findVertex(current)->getLongitude());

            }
            prev = current;
        }
    }
    return cost;
}

// Heuristic approach to solve TSP
/**
 * @brief Heuristic approach to solve the Traveling Salesman Problem (TSP).
 *
 * @details This method first finds a Minimum Spanning Tree (MST), then finds the Minimum Weight Perfect Matching (MWPM) for odd degree vertices in the MST,
 * and finally forms an Eulerian graph. An Eulerian circuit is found and converted to a TSP path.
 *
 * @param g The graph representing the delivery network.
 * @return A pair containing the total cost of the TSP path and the time taken to compute it.
 * @complexity O(V^2 log V + V^3), where V is the number of vertices in the graph.
 */
std::pair<double,double> DeliveryManager::heuristicTSP(std::unique_ptr<Graph<int>>& g) {
    auto start=std::chrono::high_resolution_clock::now();
    std::ios_base::sync_with_stdio(false);
    std::vector<int> mstPredecessors = mstPrim(g,0);

    std::vector<Edge<int>*> mstEdges;
    for(int i=0;i<mstPredecessors.size();i++) {
        if(mstPredecessors[i]!=-1) {
            for(Edge<int>* e:deliveryGraph_->findVertex(mstPredecessors[i])->getAdj()) {
                if(e->getDest()->getInfo()==i) {
                    mstEdges.push_back(e);
                }
            }
        }
    }
    std::vector<Vertex<int>*> oddVertices = findOddDegreeVertices(g, mstPredecessors);
    std::vector<std::pair<Vertex<int>*, Edge<int>*>> mwpmVertices = minimumWeightPerfectMatching(oddVertices);
    std::vector<Edge<int>*> mwpmEdges;
    for(auto e:mwpmVertices) {
        mstEdges.push_back(e.second);
    }

    std::vector<Edge<int>*> eulerianEdges = combineMSTandMWPM(mstEdges, mwpmEdges);
    std::vector<int> eulerianCircuit1 = eulerianCircuit(g, eulerianEdges,0);
    double cost=calculateTSPPath(eulerianCircuit1, g,eulerianEdges);
    auto end=std::chrono::high_resolution_clock::now();
    double res_t=std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
    res_t*=1e-9;

    return  std::make_pair(cost,res_t);
}

/**
 * @brief Exact approach to solve the Traveling Salesman Problem (TSP).
 *
 * @details This method uses a similar approach as heuristicTSP but starts from a specified starting vertex.
 * It finds an MST, then finds the MWPM for odd degree vertices in the MST, and finally forms an Eulerian graph.
 * An Eulerian circuit is found and converted to a TSP path.
 *
 * @param g The graph representing the delivery network.
 * @param s The starting vertex.
 * @return A pair containing the total cost of the TSP path and the time taken to compute it.
 * @complexity O(V^2 log V + V^3), where V is the number of vertices in the graph.
 */
std::pair<double,double> DeliveryManager::realtsp(std::unique_ptr<Graph<int>>& g, int s) {
    auto start=std::chrono::high_resolution_clock::now();
    std::ios_base::sync_with_stdio(false);
    std::vector<int> mstPredecessors = mstPrim(g,s);

    std::vector<Edge<int>*> mstEdges;
    for(int i=0;i<mstPredecessors.size();i++) {
        if(mstPredecessors[i]!=-1) {
            for(Edge<int>* e:deliveryGraph_->findVertex(mstPredecessors[i])->getAdj()) {
                if(e->getDest()->getInfo()==i) {
                    mstEdges.push_back(e);
                }
            }
        }
    }
    std::vector<Vertex<int>*> oddVertices = findOddDegreeVertices(g, mstPredecessors);
    std::vector<std::pair<Vertex<int>*, Edge<int>*>> mwpmVertices = minimumWeightPerfectMatching(oddVertices);
    std::vector<Edge<int>*> mwpmEdges;
    for(auto e:mwpmVertices) {
        mstEdges.push_back(e.second);
    }

    std::vector<Edge<int>*> eulerianEdges = combineMSTandMWPM(mstEdges, mwpmEdges);
    std::vector<int> eulerianCircuit1 = eulerianCircuit(g, eulerianEdges,s);
    double cost=calculateTSPPath(eulerianCircuit1, g,eulerianEdges);
    auto end=std::chrono::high_resolution_clock::now();
    double res_t=std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
    res_t*=1e-9;

    return  std::make_pair(cost,res_t);
}


