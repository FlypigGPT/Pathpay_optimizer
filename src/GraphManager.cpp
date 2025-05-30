#include "GraphManager.h"
#include <fstream>
#include <iostream>
GraphManager::GraphManager() : edgesFile("data/edges.txt"), institutionsFile("data/nodes.txt") {}
void GraphManager::addInstitution(const std::string& name, const std::string& country) {
    std::ofstream outFile(institutionsFile, std::ios::app);
    if (!outFile) {
        std::cerr << "Unable to open institution file." << std::endl;
        return;
    }
    outFile << name << " " << country << std::endl;
    std::cout << "Institution added successfully." << std::endl;
}
void GraphManager::viewInstitutions() {
    std::ifstream inFile(institutionsFile);
    if (!inFile) {
        std::cerr << "Unable to read institution file." << std::endl;
        return;
    }
    std::string name, country;
    std::cout << "Institution list:" << std::endl;
    while (inFile >> name >> country) {
        std::cout << "Institution: " << name << "  Country: " << country << std::endl;
    }
}
void GraphManager::addEdge(const std::string& from, const std::string& to, double feeRate, const std::string& fromCur, const std::string& toCur) {
    std::ofstream outFile(edgesFile, std::ios::app);
    if (!outFile) {
        std::cerr << "Unable to open edge file." << std::endl;
        return;
    }
    outFile << from << " " << to << " " << feeRate << " " << fromCur << " " << toCur << std::endl;
    std::cout << "Financial flow path added successfully." << std::endl;
}
void GraphManager::viewEdges() {
    std::ifstream inFile(edgesFile);
    if (!inFile) {
        std::cerr << "Unable to read edge file." << std::endl;
        return;
    }
    std::string from, to, fromCur, toCur;
    double fee;
    std::cout << "Financial flow path list:" << std::endl;
    while (inFile >> from >> to >> fee >> fromCur >> toCur) {
        std::cout << "From " << from << " to " << to << "  Fee rate: " << fee << "  Currency: " << fromCur << " -> " << toCur << std::endl;
    }
}
void GraphManager::loadGraph() {
    graph.clear();
    std::ifstream inFile(edgesFile);
    std::string from, to, fromCur, toCur;
    double fee;
    while (inFile >> from >> to >> fee >> fromCur >> toCur) {
        graph[from].push_back({ to, fee, fromCur, toCur });
    }
}
void GraphManager::findBestPath(const std::string& start, const std::string& end, double amount, RateManager& rateManager) {
    loadGraph();
    std::unordered_map<std::string, double> cost;
    std::unordered_map<std::string, std::string> prev;
    auto cmp = [&](const std::pair<std::string, double>& a, const std::pair<std::string, double>& b) {
        return a.second < b.second;
    };
    std::priority_queue<std::pair<std::string, double>, std::vector<std::pair<std::string, double>>, decltype(cmp)> pq(cmp);
    for (const auto& pair : graph) {
        cost[pair.first] = 0.0;
    }
    cost[start] = amount;
    pq.push({ start, amount });
    while (!pq.empty()) {
        auto [current, currentAmount] = pq.top(); pq.pop();
        if (current == end) break;
        for (const auto& edge : graph[current]) {
            double rate = rateManager.getRateOnline(edge.fromCurrency, edge.toCurrency);
            if (rate < 0) continue;
            double newAmount = currentAmount * rate * (1 - edge.feeRate);
            if (newAmount > cost[edge.to]) {
                cost[edge.to] = newAmount;
                prev[edge.to] = current;
                pq.push({ edge.to, newAmount });
            }
        }
    }
    if (cost[end] == 0.0) {
        std::cout << "Unable to reach the target institution." << std::endl;
        return;
    }
    std::vector<std::string> path;
    for (std::string at = end; !at.empty(); at = prev[at]) {
        path.insert(path.begin(), at);
        if (prev.find(at) == prev.end()) break;
    }
    std::cout << "Optimal path: ";
    for (const auto& node : path) std::cout << node << " ";
    std::cout << "\nFinal amount: " << cost[end] << std::endl;
}
