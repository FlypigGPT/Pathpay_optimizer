#include "utils.h"
#include <fstream>
std::string getCurrencyFromEdges(const std::string& institution, bool isSource) {
    std::ifstream inFile("data/edges.txt");
    std::string from, to, fromCur, toCur;
    double fee;
    while (inFile >> from >> to >> fee >> fromCur >> toCur) {
        if (isSource && from == institution) {
            return fromCur;
        } else if (!isSource && to == institution) {
            return toCur;
        }
    }
    return "Unknown";
}
