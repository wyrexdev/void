#include <curl/curl.h>
#include <iconv.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

class Fetcher {
public:
    static std::string get(const std::string& addr);

private:
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);
    static std::string convertToUTF8(const std::string& input, const std::string& fromCharset);
};