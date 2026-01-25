#pragma once

#include <curl/curl.h>
#include <iconv.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

#include "Core/Net/Resource/Resource.hpp"

#include "Utils/String/String.hpp"

class NetworkLoader
{
public:
    static Core::Resource get(const std::string &addr);
    static std::string urlDecode(const std::string &input);
    static bool isAbsoluteURL(const std::string &url);
    static std::string getOrigin(const std::string &url);
    static std::string getBaseDir(const std::string &url);

private:
    static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp);
    static size_t headerCallback(char *buffer, size_t size, size_t nitems, void *userdata);

    static bool isValidUTF8(const std::string &str);
    static std::string ensureUTF8(const std::string &content, const char *content_type);
    static std::string convertToUTF8(const std::string &input, const std::string &fromCharset);
    static void parseContentType(const char *ct, std::string &mime, std::string &charset);
};