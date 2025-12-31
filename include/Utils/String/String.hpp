#pragma once

#include <string>
#include <vector>

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

    static std::vector<std::string> split(const std::string &s, std::string seperator)
    {
        std::vector<std::string> output;

        std::string::size_type prev_pos = 0, pos = 0;

        while ((pos = s.find(seperator, pos)) != std::string::npos)
        {
            std::string substring(s.substr(prev_pos, pos - prev_pos));

            output.push_back(substring);

            prev_pos = ++pos;
        }

        output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word

        return output;
    }
};