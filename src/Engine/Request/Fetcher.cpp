#include "Engine/Request/Fetcher.hpp"

size_t Fetcher::writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string Fetcher::get(const std::string& addr) {
    CURL* curl = curl_easy_init();
    std::string readBuffer;

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, addr.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Fetcher::writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        CURLcode res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;

        curl_easy_cleanup(curl);
    }

    return readBuffer;
}