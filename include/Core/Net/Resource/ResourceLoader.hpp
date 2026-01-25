#pragma once

#include "Engine/Request/NetworkLoader.hpp"

#include "Utils/String/String.hpp"

#include "Core/Net/Resource/Resource.hpp"

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

namespace Core
{
    class ResourceLoader
    {
    public:
        Resource loadResource(std::string path);
    };
} // namespace name
