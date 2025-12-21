#pragma once

#include <string>

class String
{
public:
    static std::string replaceAll(
        std::string_view src,
        std::string_view from,
        std::string_view to)
    {
        std::string out;
        out.reserve(src.size());

        size_t pos = 0;
        while (true)
        {
            size_t found = src.find(from, pos);
            if (found == std::string_view::npos)
            {
                out.append(src.substr(pos));
                break;
            }

            out.append(src.substr(pos, found - pos));
            out.append(to);
            pos = found + from.size();
        }

        return out;
    }
};