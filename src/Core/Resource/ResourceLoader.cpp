#include "Core/Resource/ResourceLoader.hpp"

namespace Core
{
    ResourceLoader::Resource ResourceLoader::loadResource(std::string path)
    {
        ResourceLoader::Resource res;

        std::string origin = String::split(path, '://')[0];

        res.url = path;

        if (origin == "file")
        {
            res.origin = ResourceOrigin::FILE;
            
        }
        else if (origin == "http" || origin == "https")
        {
            res.origin = origin == "http" ? ResourceOrigin::HTTP : ResourceOrigin::HTTPS;
            res.body = Fetcher::get(path);
        }

        return res;
    }
} // namespace Core
