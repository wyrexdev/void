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

class Entity : protected QOpenGLFunctions_4_5_Compatibility
{
public:
    Entity();
    ~Entity();

    GLuint VAO = 0, VBO = 0, EBO = 0;

    void setPosition(glm::vec3 pos);
    void setPosition(float x, float y, float z);
    void setPosition(float x, float y);
    void setPosX(float x);
    void setPosY(float y);
    void setZIndex(float z);

    void setRotate(glm::vec3 rotate);
    void setRotate(float x, float y, float z);
    void setRotate(float x, float y);
    void setRotX(float x);
    void setRotY(float y);
    void setRotZ(float z);

    void setScale(glm::vec3 scale);
    void setWidth(float x);
    void setHeight(float y);
    void setScaleZ(float z);

    glm::vec3 getPosition();
    glm::vec3 getRotation();
    glm::vec3 getScale();

    void start();
    void draw();
    void update();

    void setText(const std::string &newText);
    void setColor(const glm::vec4 &newColor);
    void setFontSize(float size);
    void loadFont(const std::string &path, float size);

    const char *vertexShaderSource = R"(
        #version 450 core
        layout(location = 0) in vec2 aPos;
        layout(location = 1) in vec2 aUV;
        layout(location = 2) in vec4 aColor;
        layout(location = 3) in vec4 bColor;
        
        out vec2 TexCoord;
        out vec4 FragColor;
        out vec4 BackgroundColor;
        
        uniform mat4 projection;
        
        void main()
        {
            gl_Position = projection * vec4(aPos, 0.0, 1.0);
            TexCoord = aUV;
            FragColor = aColor;
            BackgroundColor = bColor;
        }
    )";

    const char *fragmentShaderSource = R"(
        #version 450 core
        in vec2 TexCoord;
        in vec4 FragColor;
        in vec4 BackgroundColor;
        out vec4 FragColorOut;

        uniform sampler2D textTexture;
        
        void main()
        {
            float alpha = texture(textTexture, TexCoord).r;
            vec4 textColor = vec4(FragColor.rgb, FragColor.a * alpha);
            FragColorOut = mix(BackgroundColor, textColor, textColor.a);
        }
    )";

    struct Vertex
    {
        glm::vec2 pos;
        glm::vec2 uv;
        glm::vec4 color;
        glm::vec4 backgroundColor;
    };

private:
    glm::vec3 pos;
    glm::vec3 rotate;
    glm::vec3 scale;

    std::string text;

    glm::vec4 color;
    glm::vec4 backgroundColor;

    std::string fontPath;
    float fontSize;
    float fontWeight;

    GLuint fontTexture;
    stbtt_bakedchar cdata[96];
    GLuint shaderProgram;

    void compileShaders();
    GLuint compileShader(GLenum type, const char *source);

    std::vector<float> vertices = {
        -1, -1, 0, // Left Bottom
        -1, 1, 0,  // Left Top
        1, 1, 0,   // Right Top
        1, -1, 0   // Right Bottom
    };

    std::vector<unsigned int> drawOrder = {
        0, 1, 2, // Left Triangle
        3, 1, 2  // Right Triangle
    };
};