#pragma once

#include "glm/glm/glm.hpp"
#include "Stb/Truetype.h"
#include "Engine/Renderer/Utils/Color.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"

#include <GL/gl.h>

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions_4_5_Compatibility>
#include <QFile>
#include <vector>
#include <string>
#include <unordered_map>

struct Vertex
{
    glm::vec2 position;
    glm::vec2 uv;
    glm::vec4 color;
    glm::vec4 backgroundColor;
};

struct ElementTypes
{
    inline static int Block = 0;
    inline static int Inline = 1;
};

class Entity : protected QOpenGLFunctions_4_5_Compatibility
{
public:
    Entity();
    ~Entity();

    void addEntity(Entity *entity);
    glm::vec2 getEntitiesSize();

    void setType(int type);
    int getType();

    void setPosition(glm::vec3 pos);
    void setPosition(float x, float y, float z = 0.0f);
    void setPosX(float x);
    void setPosY(float y);
    void setZIndex(float z);

    void setRotate(glm::vec3 rotate);
    void setRotate(float x, float y, float z = 0.0f);
    void setRotX(float x);
    void setRotY(float y);
    void setRotZ(float z);

    void setScale(glm::vec3 scale);
    void setWidth(float w);
    void setHeight(float h);
    void setScaleZ(float z);

    void setPadding(glm::vec4 padding);
    void setPadding(float top, float right, float bottom, float left);
    void setTopPadding(float value);
    void setRightPadding(float value);
    void setBottomPadding(float value);
    void setLeftPadding(float value);

    void setMargin(glm::vec4 margin);
    void setMargin(float top, float right, float bottom, float left);
    void setTopMargin(float value);
    void setRightMargin(float value);
    void setBottomMargin(float value);
    void setLeftMargin(float value);

    glm::vec3 getPosition();
    glm::vec3 getRotation();
    glm::vec3 getScale();

    glm::vec4 getPadding();
    glm::vec4 getMargin();

    void setText(const std::string &newText);
    void setColor(const glm::vec4 &newColor);
    void setBackgroundColor(const glm::vec4 &newColor);
    void setFontSize(float size);
    void setFont(const std::string &path);

    void setBorderRadius(float radius);
    void enableRoundedCorners(bool enable);

    void updateScaleWithPadding();
    void updateChildPositions();

    glm::vec3 getTotalSize();
    glm::vec2 getPositionWithMargin();

    glm::vec2 calculateContentSize();

    void start();
    void draw();
    void update();

    virtual void onStart() {}
    virtual void onDraw() {}
    virtual void onUpdate() {}

private:
    void loadFont(const std::string &path, float size);
    void compileShaders();
    GLuint compileShader(GLenum type, const char *source);

    glm::vec3 originalScale;
    glm::vec3 contentSize;

    glm::vec3 totalSize;

    void updateTotalSize();

    int type = ElementTypes::Inline;

    std::vector<Entity *> entities;

    std::string text;
    glm::vec4 color;
    glm::vec4 backgroundColor;
    glm::vec4 padding;
    glm::vec4 margin;
    float fontSize;
    std::string fontPath;

    glm::vec3 pos;
    glm::vec3 rotate;
    glm::vec3 scale;

    GLuint fontTexture;
    GLuint shaderProgram;
    GLuint VAO, VBO;
    stbtt_bakedchar cdata[96];

    float borderRadius;
    bool enableBorderRadius;

    const char *vertexShaderSource = R"(
        #version 450 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec2 aTexCoord;
        layout (location = 2) in vec4 aColor;
        layout (location = 3) in vec4 aBackgroundColor;

        out vec2 TexCoord;
        out vec4 FragColor;
        out vec4 BackgroundColor;
        out vec2 FragPos;

        uniform mat4 projection;

        void main()
        {
            gl_Position = projection * vec4(aPos, 0.0, 1.0);
            FragPos = aPos;
            TexCoord = aTexCoord;
            FragColor = aColor;
            BackgroundColor = aBackgroundColor;
        }
    )";

    const char *fragmentShaderSource = R"(
        #version 450 core
        in vec2 TexCoord;
        in vec4 FragColor;
        in vec4 BackgroundColor;
        in vec2 FragPos;
        out vec4 FragColorOut;

        uniform sampler2D textTexture;
        uniform bool hasTexture;
        uniform vec4 rectBounds;
        uniform float borderRadius;
        uniform bool enableBorderRadius;

        float roundedBoxSDF(vec2 p, vec2 b, float r) {
            vec2 d = abs(p) - b + vec2(r);
            return min(max(d.x, d.y), 0.0) + length(max(d, 0.0)) - r;
        }

        void main()
        {
            if (hasTexture) {
                float alpha = texture(textTexture, TexCoord).r;
                vec4 textColor = vec4(FragColor.rgb, FragColor.a * alpha);
            
                if (alpha < 0.01) discard;
            
                float finalAlpha = textColor.a + BackgroundColor.a * (1.0 - textColor.a);
                if (finalAlpha < 0.01) discard;
            
                vec3 finalColor = (textColor.rgb * textColor.a + 
                                 BackgroundColor.rgb * BackgroundColor.a * (1.0 - textColor.a)) / finalAlpha;
            
                FragColorOut = vec4(finalColor, finalAlpha);
            } else {
                if (enableBorderRadius && borderRadius > 0.0) {
                    vec2 center = rectBounds.xy + rectBounds.zw * 0.5;
                    vec2 halfSize = rectBounds.zw * 0.5;
                    vec2 pixelPos = FragPos;
                
                    vec2 normalizedPos = (pixelPos - center) / halfSize;
                    vec2 boxSize = vec2(1.0);
                    float radius = borderRadius / min(rectBounds.z, rectBounds.w);
                
                    float distance = roundedBoxSDF(normalizedPos, boxSize, radius);
                
                    float alpha = 1.0 - smoothstep(0.0, 0.01, distance);
                
                    if (alpha < 0.01) discard;
                    FragColorOut = vec4(BackgroundColor.rgb, BackgroundColor.a * alpha);
                } else {
                    FragColorOut = BackgroundColor;
                }
            }
        }
    )";
};