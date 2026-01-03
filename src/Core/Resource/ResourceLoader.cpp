#include "Core/Resource/ResourceLoader.hpp"

namespace Core
{
    Core::Resource ResourceLoader::loadResource(std::string path)
    {
        Core::Resource res = NetworkLoader::get(path);

        if(res.mimeType == "image/png") {
            res.body = "<html><head><meta name=\"viewport\" content=\"width=device-width; height=device-height;\"><link rel=\"stylesheet\" href=\"resource://content-accessible/ImageDocument.css\"><link rel=\"stylesheet\" href=\"resource://content-accessible/TopLevelImageDocument.css\"><title>logo.png (PNG Image, 524&nbsp;×&nbsp;524 pixels)</title></head><body><img src=\"https://cdn.vobrow.com/logo.png\" alt=\"https://cdn.vobrow.com/logo.png\" class=\"transparent\"></body></html>";
        }

        return res;
    }
} // namespace Core
