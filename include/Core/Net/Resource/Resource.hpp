#pragma once

#include <string>
#include <unordered_map>

namespace Core
{
    enum class ResourceOrigin
    {
        FILE,
        HTTP,
        HTTPS
    };

    struct Resource
    {
        ResourceOrigin origin;

        std::string url;
        std::unordered_map<std::string, std::string> headers;

        std::string mimeType;
        std::string charset;

        std::string body;
        int statusCode = 0;
    };
}
