#include "Core/Resource/ResourceLoader.hpp"

namespace Core
{
    ResourceLoader::Resource ResourceLoader::loadResource(std::string path)
    {
        ResourceLoader::Resource res = NetworkLoader::get(path);;

        return res;
    }
} // namespace Core
