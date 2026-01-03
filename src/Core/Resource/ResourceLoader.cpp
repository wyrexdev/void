#include "Core/Resource/ResourceLoader.hpp"

namespace Core
{
    Core::Resource ResourceLoader::loadResource(std::string path)
    {
        Core::Resource res = NetworkLoader::get(path);;

        return res;
    }
} // namespace Core
