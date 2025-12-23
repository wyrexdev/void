#pragma once

#include <curl/curl.h>
#include <iconv.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

class Fetcher
{
public:
    static std::string get(const std::string &addr);

    bool isValidUTF8(const std::string &str);
    std::string ensureUTF8(const std::string &content, const char *content_type);

private:
    static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp);
    static std::string convertToUTF8(const std::string &input, const std::string &fromCharset);
};