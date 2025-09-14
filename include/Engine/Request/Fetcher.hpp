#include <string>
#include <iostream>
#include <curl/curl.h>

class Fetcher
{
public:
    std::string get(const std::string& addr);

private:
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);
};