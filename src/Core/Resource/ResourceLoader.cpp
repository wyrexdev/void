#include "Core/Resource/ResourceLoader.hpp"

namespace Core
{
    Core::Resource ResourceLoader::loadResource(std::string path)
    {
        Core::Resource res = NetworkLoader::get(path);

        if(res.mimeType == "image/png") {
            res.body = "<img width='100' height='100' src='" + res.url + "'>";
        }

        return res;
    }
} // namespace Core
