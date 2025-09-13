#pragma once
#include <string>

class Theme {
public:
    static void setTheme(std::string theme);

    struct Style {
        std::string background;
        std::string tab;
        std::string tabHover;
        std::string surface;
        std::string onSurface;
        std::string text;
        std::string textHover;
        int borderRadius;
    };

    static Style style;
};
