#include "Engine/Renderer/OpenGL/Entities/Entity.hpp"
#include "Engine/Renderer/OpenGL/Utils/Screen.hpp"

Entity::Entity() : text(""),
                   color(1.0f, 1.0f, 1.0f, 1.0f),
                   backgroundColor(0.8f, 0.8f, 0.8f, 1.0f),
                   fontSize(24.0f),
                   fontTexture(0),
                   shaderProgram(0),
                   VAO(0), VBO(0),
                   borderRadius(0.0f),
                   enableBorderRadius(false),
                   originalScale(100.0f, 50.0f, 0.0f),
                   contentSize(0.0f, 0.0f, 0.0f),
                   fontLoaded(false)
{
    fontPath = ":/fonts/nunito.ttf";
    pos = glm::vec3(0.0f, 0.0f, 0.0f);
    rotate = glm::vec3(0.0f);
    scale = glm::vec3(1000.0f, 40.0f, 0.0f);
    padding = glm::vec4(0.0f);
    margin = glm::vec4(0.0f);
    updateTotalSize();
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

void Entity::updateScaleWithPadding()
{
    this->scale.x = originalScale.x + padding.y + padding.w;
    this->scale.y = originalScale.y + padding.x + padding.z;
    updateTotalSize();
}

glm::vec2 Entity::calculateContentSize()
{
    if (!text.empty() && fontLoaded)
    {
        float textWidth = 0.0f;
        float textHeight = fontSize;

        for (char c : text)
        {
            if (c >= 32 && c < 128)
            {
                stbtt_bakedchar *charData = &cdata[c - 32];
                textWidth += charData->xadvance;
            }
        }

        return glm::vec2(textWidth, textHeight);
    }
    else if (!entities.empty())
    {
        return getEntitiesSize();
    }

    return glm::vec2(originalScale.x, originalScale.y);
}

void Entity::addEntity(Entity *entity)
{
    entities.push_back(entity);
    updateChildPositions();
}

void Entity::setType(int type)
{
    this->type = type;
}

int Entity::getType()
{
    return type;
}

void Entity::setPosition(glm::vec3 pos)
{
    this->pos = pos;
    updateChildPositions();
}

void Entity::setPosition(float x, float y, float z)
{
    this->pos = glm::vec3(x, y, z);
    updateChildPositions();
}

void Entity::setPosX(float x)
{
    this->pos.x = x;
    updateChildPositions();
}

void Entity::setPosY(float y)
{
    this->pos.y = y;
    updateChildPositions();
}

void Entity::setZIndex(float z) { this->pos.z = z; }

void Entity::setRotate(glm::vec3 rotate) { this->rotate = rotate; }
void Entity::setRotate(float x, float y, float z) { this->rotate = glm::vec3(x, y, z); }
void Entity::setRotX(float x) { this->rotate.x = x; }
void Entity::setRotY(float y) { this->rotate.y = y; }
void Entity::setRotZ(float z) { this->rotate.z = z; }

void Entity::setScale(glm::vec3 newScale)
{
    this->originalScale = newScale;
    updateScaleWithPadding();
}

void Entity::setWidth(float w)
{
    this->originalScale.x = w;
    updateScaleWithPadding();
}

void Entity::setHeight(float h)
{
    this->originalScale.y = h;
    updateScaleWithPadding();
}

void Entity::setScaleZ(float z) { this->scale.z = z; }

void Entity::setPadding(glm::vec4 newPadding)
{
    this->padding = newPadding;
    updateScaleWithPadding();
    updateChildPositions();
}

void Entity::setPadding(float top, float right, float bottom, float left)
{
    setPadding(glm::vec4(top, right, bottom, left));
}

void Entity::setTopPadding(float value)
{
    padding.x = value;
    updateScaleWithPadding();
    updateChildPositions();
}

void Entity::setRightPadding(float value)
{
    padding.y = value;
    updateScaleWithPadding();
}

void Entity::setBottomPadding(float value)
{
    padding.z = value;
    updateScaleWithPadding();
}

void Entity::setLeftPadding(float value)
{
    padding.w = value;
    updateScaleWithPadding();
    updateChildPositions();
}

glm::vec2 Entity::getPositionWithMargin()
{
    return glm::vec2(pos.x + margin.w, pos.y + margin.x);
}

void Entity::setMargin(glm::vec4 newMargin)
{
    this->margin = newMargin;
    updateTotalSize();
    updateChildPositions();
}

void Entity::setMargin(float top, float right, float bottom, float left)
{
    setMargin(glm::vec4(top, right, bottom, left));
}

void Entity::setTopMargin(float value)
{
    margin.x = value;
    updateTotalSize();
    updateChildPositions();
}

void Entity::setRightMargin(float value)
{
    margin.y = value;
    updateTotalSize();
}

void Entity::setBottomMargin(float value)
{
    margin.z = value;
    updateTotalSize();
}

void Entity::setLeftMargin(float value)
{
    margin.w = value;
    updateTotalSize();
    updateChildPositions();
}

glm::vec4 Entity::getMargin()
{
    return margin;
}

void Entity::updateTotalSize()
{
    this->totalSize.x = scale.x + margin.y + margin.w;
    this->totalSize.y = scale.y + margin.x + margin.z;
    this->totalSize.z = scale.z;
}

glm::vec3 Entity::getTotalSize()
{
    return totalSize;
}

void Entity::updateChildPositions()
{
    float startX = pos.x + margin.w + padding.w;
    float startY = pos.y + margin.x + padding.x;
    float currentX = startX;
    float currentY = startY;

    for (Entity *entity : entities)
    {
        if (entity->getType() == ElementTypes::Inline)
        {
            entity->setPosition(currentX, currentY);
            currentX += entity->getTotalSize().x;
        }
        else
        {
            entity->setPosition(startX, currentY);
            currentY += entity->getTotalSize().y;
        }
    }
}

glm::vec2 Entity::getEntitiesSize()
{
    glm::vec2 size(0.0f);

    if (entities.empty())
    {
        return size;
    }

    float maxWidth = 0.0f;
    float totalHeight = 0.0f;
    float currentLineWidth = 0.0f;
    float currentLineHeight = 0.0f;

    for (Entity *entity : entities)
    {
        glm::vec2 entitySize = entity->getTotalSize();

        if (entity->getType() == ElementTypes::Inline)
        {
            currentLineWidth += entitySize.x;
            currentLineHeight = glm::max(currentLineHeight, entitySize.y);
        }
        else
        {
            maxWidth = glm::max(maxWidth, currentLineWidth);
            totalHeight += currentLineHeight;

            currentLineWidth = entitySize.x;
            currentLineHeight = entitySize.y;
        }
    }

    maxWidth = glm::max(maxWidth, currentLineWidth);
    totalHeight += currentLineHeight;

    return glm::vec2(maxWidth, totalHeight);
}

glm::vec3 Entity::getPosition() { return pos; }
glm::vec3 Entity::getRotation() { return rotate; }
glm::vec3 Entity::getScale() { return scale; }

glm::vec4 Entity::getPadding() { return padding; }

void Entity::setBorderRadius(float radius)
{
    borderRadius = glm::max(0.0f, radius);
    enableRoundedCorners(true);
}
void Entity::enableRoundedCorners(bool enable) { enableBorderRadius = enable; }

void Entity::setText(const std::string &newText) { text = newText; }
void Entity::setColor(const glm::vec4 &newColor) { color = newColor; }
void Entity::setBackgroundColor(const glm::vec4 &newColor) { backgroundColor = newColor; }
void Entity::setFontSize(float size)
{
    fontSize = size;
    if (!fontPath.empty())
        loadFont(fontPath, size);
}
void Entity::setFont(const std::string &path)
{
    fontPath = path;
    loadFont(path, fontSize);
}

void Entity::start()
{
    isInitalized = true;

    initializeOpenGLFunctions();

    onStart();

    if (!fontPath.empty())
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

    onDraw();

    glUseProgram(shaderProgram);

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(ScreenUtils::width),
                                      static_cast<float>(ScreenUtils::height), 0.0f);

    GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    renderBackground();
    renderText();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
    glDisable(GL_BLEND);
}

void Entity::renderBackground()
{
    if (backgroundColor.a <= 0.0f)
        return;

    std::vector<Vertex> vertices;

    float x = pos.x + margin.w;
    float y = pos.y + margin.x;
    float w = scale.x;
    float h = scale.y;

    GLuint rectBoundsLoc = glGetUniformLocation(shaderProgram, "rectBounds");
    GLuint borderRadiusLoc = glGetUniformLocation(shaderProgram, "borderRadius");
    GLuint enableBorderRadiusLoc = glGetUniformLocation(shaderProgram, "enableBorderRadius");
    GLuint hasTextureLoc = glGetUniformLocation(shaderProgram, "hasTexture");
    GLuint thicknessLoc = glGetUniformLocation(shaderProgram, "fontThickness");

    glUniform4f(rectBoundsLoc, x, y, w, h);
    glUniform1f(borderRadiusLoc, borderRadius);
    glUniform1i(enableBorderRadiusLoc, enableBorderRadius);
    glUniform1i(hasTextureLoc, 0);
    glUniform1f(thicknessLoc, 1.8f);

    vertices.insert(vertices.end(), {{{x, y}, {0.0f, 0.0f}, color, backgroundColor},
                                     {{x + w, y}, {1.0f, 0.0f}, color, backgroundColor},
                                     {{x + w, y + h}, {1.0f, 1.0f}, color, backgroundColor},
                                     {{x, y}, {0.0f, 0.0f}, color, backgroundColor},
                                     {{x + w, y + h}, {1.0f, 1.0f}, color, backgroundColor},
                                     {{x, y + h}, {0.0f, 1.0f}, color, backgroundColor}});

    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void Entity::renderText()
{
    if (text.empty() || !fontTexture || !fontLoaded)
        return;

    std::vector<Vertex> textVertices;
    float penX = pos.x + margin.w + padding.w;
    float penY = pos.y + margin.x + padding.x + fontSize;

    GLuint hasTextureLoc = glGetUniformLocation(shaderProgram, "hasTexture");
    GLuint enableBorderRadiusLoc = glGetUniformLocation(shaderProgram, "enableBorderRadius");

    glUniform1i(hasTextureLoc, 1);
    glUniform1i(enableBorderRadiusLoc, 0);

    for (char c : text)
    {
        int charIndex = -1;

        if (c >= 32 && c < 128)
        {
            charIndex = c - 32;
        }

        // Turkish
        else if (c == -61)
        {
            continue;
        }
        else if (c == -89)
        { // ç
            charIndex = 128;
        }
        else if (c == -79)
        { // ğ
            charIndex = 129;
        }
        else if (c == -87)
        { // ı
            charIndex = 130;
        }
        else if (c == -74)
        { // ö
            charIndex = 131;
        }
        else if (c == -68)
        { // ş
            charIndex = 132;
        }
        else if (c == -71)
        { // ü
            charIndex = 133;
        }

        else if (c == -60)
        { // UTF-8
            continue;
        }
        else if (c == -97)
        { // Ç
            charIndex = 134;
        }
        else if (c == -79)
        { // Ğ
            charIndex = 135;
        }
        else if (c == -73)
        { // İ
            charIndex = 136;
        }
        else if (c == -78)
        { // Ö
            charIndex = 137;
        }
        else if (c == -66)
        { // Ş
            charIndex = 138;
        }
        else if (c == -69)
        { // Ü
            charIndex = 139;
        }

        if (charIndex >= 0 && charIndex < 160)
        {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(cdata, 1024, 1024, charIndex, &penX, &penY, &q, 1);

            textVertices.insert(textVertices.end(), {{{q.x0, q.y0}, {q.s0, q.t0}, color, backgroundColor},
                                                     {{q.x1, q.y0}, {q.s1, q.t0}, color, backgroundColor},
                                                     {{q.x1, q.y1}, {q.s1, q.t1}, color, backgroundColor},
                                                     {{q.x0, q.y0}, {q.s0, q.t0}, color, backgroundColor},
                                                     {{q.x1, q.y1}, {q.s1, q.t1}, color, backgroundColor},
                                                     {{q.x0, q.y1}, {q.s0, q.t1}, color, backgroundColor}});
        }
        else
        {
            penX += fontSize * 0.5f;
        }
    }

    if (!textVertices.empty())
    {
        glBufferSubData(GL_ARRAY_BUFFER, 0, textVertices.size() * sizeof(Vertex), textVertices.data());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fontTexture);

        glDrawArrays(GL_TRIANGLES, 0, textVertices.size());
    }
}

void Entity::update()
{
    onUpdate();
}

void Entity::loadFont(const std::string &path, float size)
{
    fontSize = size;

    QFile file(QString::fromStdString(path));
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning("Font cannot open: %s", path.c_str());
        fontLoaded = false;
        return;
    }

    QByteArray fontData = file.readAll();
    file.close();

    const int bitmapWidth = 1024, bitmapHeight = 1024;
    std::vector<unsigned char> bitmap(bitmapWidth * bitmapHeight, 0);

    int firstChar = 32;
    int numChars = 160;

    int result = stbtt_BakeFontBitmap(
        reinterpret_cast<const unsigned char *>(fontData.data()),
        0,
        size,
        bitmap.data(),
        bitmapWidth,
        bitmapHeight,
        firstChar,
        numChars,
        cdata);

    if (result <= 0)
    {
        qWarning("Font baking failed: %d", result);
        fontLoaded = false;
        return;
    }

    createFontTexture(bitmap, bitmapWidth, bitmapHeight);
    fontLoaded = true;
}

void Entity::createFontTexture(const std::vector<unsigned char> &bitmap, int width, int height)
{
    if (fontTexture)
        glDeleteTextures(1, &fontTexture);

    glGenTextures(1, &fontTexture);
    glBindTexture(GL_TEXTURE_2D, fontTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0,
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