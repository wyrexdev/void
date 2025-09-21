#include "Engine/Renderer/OpenGL/Entities/Entity.hpp"
#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions_4_5_Compatibility>
#include <QFile>

#include "Stb/Truetype.h"
#include "glm/glm/gtc/matrix_transform.hpp"

#include <vector>
#include <string>

class Text : public Entity, protected QOpenGLFunctions_4_5_Compatibility
{
public:
    Text();
    ~Text();
    
    void start() override;
    void draw() override;
    void update() override;

    void setText(const std::string& newText);
    void setColor(const glm::vec4& newColor);
    void setFontSize(float size);
    void loadFont(const std::string& path, float size);

private:
    std::string text;
    glm::vec4 color;
    
    std::string fontPath;
    float fontSize;
    float fontWeight;

    GLuint fontTexture;
    stbtt_bakedchar cdata[96];
    GLuint shaderProgram;
    
    struct Vertex
    {
        glm::vec2 pos;
        glm::vec2 uv;
        glm::vec4 color;
    };

    void compileShaders();
    GLuint compileShader(GLenum type, const char* source);
};