#pragma once

#include "Engine/Request/Fetcher.hpp"

#include "Utils/String/String.hpp"

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
            std::string body;

            int statusCode;
            std::unordered_map<std::string, std::string> headers;
        };
        
        Resource loadResource(std::string path);
    };
} // namespace name
