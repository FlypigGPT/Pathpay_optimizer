#include "RateManager.h"
#include <iostream>
#include <json/json.h>
#include <curl/curl.h>
#include <sstream>
RateManager::RateManager() : apiKey("RGBaQ6phT9oTNYbWnfsWacN28mahcOJj") {}
size_t RateManager::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}
double RateManager::getRateOnline(const std::string& from, const std::string& to) {
    std::string url = "https://api.apilayer.com/currency_data/convert?from=" + from + "&to=" + to + "&amount=1";
    CURL* curl = curl_easy_init();
    std::string readBuffer;
    if (curl) {
        struct curl_slist* headers = NULL;
        std::string authHeader = "apikey: " + apiKey;
        headers = curl_slist_append(headers, authHeader.c_str());
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        CURLcode res = curl_easy_perform(curl);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        if (res != CURLE_OK) {
            std::cerr << "Request failed: " << curl_easy_strerror(res) << std::endl;
            return -1.0;
        }
        Json::Value jsonData;
        Json::CharReaderBuilder builder;
        std::string errs;
        std::istringstream s(readBuffer);
        if (Json::parseFromStream(builder, s, &jsonData, &errs)) {
            if (jsonData["success"].asBool()) {
                double rate = jsonData["result"].asDouble();
                std::cout << "Exchange rate retrieved successfully: " << from << " -> " << to << " = " << rate << std::endl;
                return rate;
            } else {
                std::cerr << "API returned failure." << std::endl;
                return -1.0;
            }
        } else {
            std::cerr << "Failed to parse JSON: " << errs << std::endl;
            return -1.0;
        }
    }
    return -1.0;
}
