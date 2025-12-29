#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

namespace Core
{
    enum class ResourceOrigin {
        FILE,
        HTTP,
        HTTPS
    };

    class ResourceLoader
    {
    public:
        struct Resource
        {
            ResourceOrigin origin;

            std::string url;
            std::string mimeType;
            std::vector<uint8_t> body;

            int statusCode;
            std::unordered_map<std::string, std::string> headers;
        };
        
        Resource loadResource();
    };
} // namespace name
