#ifndef RATEMANAGER_H
#define RATEMANAGER_H
#include <string>
class RateManager {
private:
    std::string apiKey;
public:
    RateManager();
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output);
    double getRateOnline(const std::string& from, const std::string& to);
};
#endif // RATEMANAGER_H
