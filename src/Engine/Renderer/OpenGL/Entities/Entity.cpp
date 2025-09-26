#include "Engine/Renderer/OpenGL/Entities/Entity.hpp"
#include "Engine/Renderer/OpenGL/Utils/Screen.hpp"

Entity::Entity() : text("Hello World"),
                   color(0.0f, 0.0f, 0.0f, 1.0f),
                   backgroundColor(0.8f, 0.8f, 0.8f, 1.0f),
                   fontSize(24.0f),
                   fontTexture(0),
                   shaderProgram(0),
                   VAO(0), VBO(0),
                   borderRadius(0.0f),
                   enableBorderRadius(false)
{
    fontPath = ":/fonts/nunito.ttf";
    pos = glm::vec3(100.0f, 100.0f, 0.0f);
    rotate = glm::vec3(0.0f);
    scale = glm::vec3(400.0f, 200.0f, 1.0f);
}

Entity::~Entity()
{
    if (fontTexture)
        glDeleteTextures(1, &fontTexture);
    if (shaderProgram)
        glDeleteProgram(shaderProgram);
    if (VAO)
        glDeleteVertexArrays(1, &VAO);
    if (VBO)
        glDeleteBuffers(1, &VBO);
}

void Entity::setPosition(glm::vec3 pos) { this->pos = pos; }
void Entity::setPosition(float x, float y, float z) { this->pos = glm::vec3(x, y, z); }
void Entity::setPosX(float x) { this->pos.x = x; }
void Entity::setPosY(float y) { this->pos.y = y; }
void Entity::setZIndex(float z) { this->pos.z = z; }

void Entity::setRotate(glm::vec3 rotate) { this->rotate = rotate; }
void Entity::setRotate(float x, float y, float z) { this->rotate = glm::vec3(x, y, z); }
void Entity::setRotX(float x) { this->rotate.x = x; }
void Entity::setRotY(float y) { this->rotate.y = y; }
void Entity::setRotZ(float z) { this->rotate.z = z; }

void Entity::setScale(glm::vec3 scale) { this->scale = scale; }
void Entity::setWidth(float w) { this->scale.x = w; }
void Entity::setHeight(float h) { this->scale.y = h; }
void Entity::setScaleZ(float z) { this->scale.z = z; }

glm::vec3 Entity::getPosition() { return pos; }
glm::vec3 Entity::getRotation() { return rotate; }
glm::vec3 Entity::getScale() { return scale; }

void Entity::setBorderRadius(float radius) { borderRadius = glm::max(0.0f, radius); }
void Entity::enableRoundedCorners(bool enable) { enableBorderRadius = enable; }

void Entity::setText(const std::string &newText) { text = newText; }
void Entity::setColor(const glm::vec4 &newColor) { color = newColor; }
void Entity::setBackgroundColor(const glm::vec4 &newColor) { backgroundColor = newColor; }
void Entity::setFontSize(float size)
{
    fontSize = size;
    loadFont(fontPath, size);
}
void Entity::setFont(const std::string &path)
{
    fontPath = path;
    loadFont(path, fontSize);
}

void Entity::start()
{
    initializeOpenGLFunctions();

    loadFont(fontPath, fontSize);
    compileShaders();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 1024, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, backgroundColor));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Entity::draw()
{
    if (!shaderProgram)
        return;

    glUseProgram(shaderProgram);

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(ScreenUtils::width),
                                      static_cast<float>(ScreenUtils::height), 0.0f);

    GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

    GLuint borderRadiusLoc = glGetUniformLocation(shaderProgram, "borderRadius");
    GLuint enableBorderRadiusLoc = glGetUniformLocation(shaderProgram, "enableBorderRadius");
    GLuint rectBoundsLoc = glGetUniformLocation(shaderProgram, "rectBounds");

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    std::vector<Vertex> vertices;

    if (backgroundColor.a > 0.0f)
    {
        float x = pos.x;
        float y = pos.y;
        float w = scale.x;
        float h = scale.y;

        glUniform4f(rectBoundsLoc, x, y, w, h);
        glUniform1f(borderRadiusLoc, borderRadius);
        glUniform1i(enableBorderRadiusLoc, enableBorderRadius);

        vertices.insert(vertices.end(), {{{x, y}, {0.0f, 0.0f}, color, backgroundColor},
                                         {{x + w, y}, {1.0f, 0.0f}, color, backgroundColor},
                                         {{x + w, y + h}, {1.0f, 1.0f}, color, backgroundColor},
                                         {{x, y}, {0.0f, 0.0f}, color, backgroundColor},
                                         {{x + w, y + h}, {1.0f, 1.0f}, color, backgroundColor},
                                         {{x, y + h}, {0.0f, 1.0f}, color, backgroundColor}});
    }

    if (!vertices.empty())
    {
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

        GLuint hasTextureLoc = glGetUniformLocation(shaderProgram, "hasTexture");
        glUniform1i(hasTextureLoc, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    }

    if (!text.empty() && fontTexture)
    {
        glUniform1i(enableBorderRadiusLoc, 0);

        std::vector<Vertex> textVertices;
        float penX = pos.x + 10.0f;
        float penY = pos.y + (scale.y / 2) + (fontSize / 3);

        for (char c : text)
        {
            if (c >= 32 && c < 128)
            {
                stbtt_aligned_quad q;
                stbtt_GetBakedQuad(cdata, 512, 512, c - 32, &penX, &penY, &q, 1);

                textVertices.insert(textVertices.end(), {{{q.x0, q.y0}, {q.s0, q.t0}, color, backgroundColor},
                                                         {{q.x1, q.y0}, {q.s1, q.t0}, color, backgroundColor},
                                                         {{q.x1, q.y1}, {q.s1, q.t1}, color, backgroundColor},
                                                         {{q.x0, q.y0}, {q.s0, q.t0}, color, backgroundColor},
                                                         {{q.x1, q.y1}, {q.s1, q.t1}, color, backgroundColor},
                                                         {{q.x0, q.y1}, {q.s0, q.t1}, color, backgroundColor}});
            }
        }

        if (!textVertices.empty())
        {
            glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                            textVertices.size() * sizeof(Vertex), textVertices.data());

            GLuint hasTextureLoc = glGetUniformLocation(shaderProgram, "hasTexture");
            glUniform1i(hasTextureLoc, 1);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, fontTexture);

            glDrawArrays(GL_TRIANGLES, vertices.size(), textVertices.size());
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
    glDisable(GL_BLEND);
}

void Entity::update()
{
}

void Entity::loadFont(const std::string &path, float size)
{
    fontSize = size;

    QFile file(QString::fromStdString(path));
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning("Font cannot open: %s", path.c_str());
        return;
    }

    QByteArray fontData = file.readAll();
    file.close();

    const int bitmapWidth = 512, bitmapHeight = 512;
    std::vector<unsigned char> bitmap(bitmapWidth * bitmapHeight, 0);

    stbtt_fontinfo font;
    if (!stbtt_InitFont(&font, reinterpret_cast<const unsigned char *>(fontData.data()), 0))
    {
        qWarning("Failed to initialize font");
        return;
    }

    float scale = stbtt_ScaleForPixelHeight(&font, size);

    std::fill(bitmap.begin(), bitmap.end(), 0);
    int result = stbtt_BakeFontBitmap(reinterpret_cast<const unsigned char *>(fontData.data()), 0,
                                      size, bitmap.data(), bitmapWidth, bitmapHeight, 32, 96, cdata);

    if (result <= 0)
    {
        qWarning("Font baking failed: %d", result);
        return;
    }

    if (fontTexture)
        glDeleteTextures(1, &fontTexture);

    glGenTextures(1, &fontTexture);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmapWidth, bitmapHeight, 0,
                 GL_RED, GL_UNSIGNED_BYTE, bitmap.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Entity::compileShaders()
{
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    if (shaderProgram)
        glDeleteProgram(shaderProgram);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        qWarning("Shader linking failed: %s", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

GLuint Entity::compileShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        qWarning("Shader compilation failed: %s", infoLog);
    }

    return shader;
}