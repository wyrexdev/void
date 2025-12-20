#pragma once

#include "Headers/Global.hpp"

class Ram
{
public:
    static std::string format_bytes(size_t bytes)
    {
        const char *units[] = {"B", "KB", "MB", "GB", "TB"};
        double size = static_cast<double>(bytes);
        int unit = 0;

        while (size >= 1024.0 && unit < 4)
        {
            size /= 1024.0;
            unit++;
        }

        std::ostringstream out;

        if (unit == 0)
        {
            out << static_cast<size_t>(size) << " " << units[unit];
        }
        else
        {
            out << static_cast<size_t>(size) << " " << units[unit];
        }

        return out.str();
    }
};