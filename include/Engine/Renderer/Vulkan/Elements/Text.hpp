#define STB_TRUETYPE_IMPLEMENTATION
#include "Stb/Truetype.h"

#include "Engine/Renderer/Vulkan/Element.hpp"

#include "Stb/Truetype.h"

#include <string>

class Text : public Element {
public:
    Text() {}

    void start () override {

    }

    void render () override {

    }

    void update () override {
        
    }

    void setText(const std::string &test) {
        update();
    }

    void setFontSize(float size) {

    }

    void setWeight(int weight) {

    }
};