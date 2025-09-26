#include "glm/glm/glm.hpp"
#include "Engine/Renderer/Utils/Color.hpp"

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions_4_5_Compatibility>
#include <QFile>

#include "Stb/Truetype.h"
#include "glm/glm/gtc/matrix_transform.hpp"

#include <vector>
#include <string>
#include <GL/gl.h>

struct Vertex
{
    glm::vec2 position;
    glm::vec2 uv;
    glm::vec4 color;
    glm::vec4 backgroundColor;
};

class Entity : protected QOpenGLFunctions_4_5_Compatibility
{
public:
    Entity();
    ~Entity();

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

    glm::vec3 getPosition();
    glm::vec3 getRotation();
    glm::vec3 getScale();

    void setText(const std::string &newText);
    void setColor(const glm::vec4 &newColor);
    void setBackgroundColor(const glm::vec4 &newColor);
    void setFontSize(float size);
    void setFont(const std::string &path);

    void start();
    void draw();
    void update();

private:
    void loadFont(const std::string &path, float size);
    void compileShaders();
    GLuint compileShader(GLenum type, const char *source);

    std::string text;
    glm::vec4 color;
    glm::vec4 backgroundColor;
    float fontSize;
    std::string fontPath;

    glm::vec3 pos;
    glm::vec3 rotate;
    glm::vec3 scale;

    GLuint fontTexture;
    GLuint shaderProgram;
    GLuint VAO, VBO;
    stbtt_bakedchar cdata[96];

    const char *vertexShaderSource = R"(
        #version 450 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec2 aTexCoord;
        layout (location = 2) in vec4 aColor;
        layout (location = 3) in vec4 aBackgroundColor;

        out vec2 TexCoord;
        out vec4 FragColor;
        out vec4 BackgroundColor;

        uniform mat4 projection;

        void main()
        {
            gl_Position = projection * vec4(aPos, 0.0, 1.0);
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
        out vec4 FragColorOut;

        uniform sampler2D textTexture;
        uniform bool hasTexture;

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
                FragColorOut = BackgroundColor;
            }
        }
    )";
};