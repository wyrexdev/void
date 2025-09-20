#define STB_TRUETYPE_IMPLEMENTATION
#include "Stb/Truetype.h"

#include "Engine/Renderer/Vulkan/Element.hpp"
#include "Engine/Renderer/Vulkan/Texture/VulkanTexture.hpp"
#include "Utils/Renderer.hpp"

#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <glm/glm.hpp>

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

private:
    std::string m_text;
    float m_fontSize;
    int m_weight;
    bool m_dirty;

    glm::vec4 m_color;
    glm::vec2 m_position;
    glm::vec2 m_textSize;

    std::vector<unsigned char> m_fontData;
    stbtt_fontinfo m_fontInfo;
    std::vector<float> m_vertices;
    std::shared_ptr<VulkanTexture> m_textureAtlas;
};