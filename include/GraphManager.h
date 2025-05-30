#ifndef GRAPHMANAGER_H
#define GRAPHMANAGER_H
#include <string>
#include <vector>
#include <unordered_map>
#include "RateManager.h"
struct Edge {
    std::string to;
    double feeRate;
    std::string fromCurrency;
    std::string toCurrency;
};
class GraphManager {
private:
    std::string edgesFile;
    std::string institutionsFile;
    std::unordered_map<std::string, std::vector<Edge>> graph;
public:
    GraphManager();
    void addInstitution(const std::string& name, const std::string& country);
    void viewInstitutions();
    void addEdge(const std::string& from, const std::string& to, double feeRate, const std::string& fromCur, const std::string& toCur);
    void viewEdges();
    void loadGraph();
    void findBestPath(const std::string& start, const std::string& end, double amount, RateManager& rateManager);
};
#endif // GRAPHMANAGER_H
