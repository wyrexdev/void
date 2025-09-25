#include "Engine/Renderer/OpenGL/Entities/Entity.hpp"

Entity::Entity() : text("asd"),
                   color(0.0f, 0.0f, 1.0f, 1.0f),
                   backgroundColor(0.0f, 1.0f, 1.0f, 1.0f),
                   fontSize(24.0f),
                   fontWeight(240.0f),
                   fontTexture(0),
                   shaderProgram(0)
{
    fontPath = ":/fonts/nano.ttf";
    pos = glm::vec3(0.0f);
    rotate = glm::vec3(0.0f);
    scale = glm::vec3(100.0f, 50.0f, 1.0f); // Varsayılan boyut
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

void Entity::setPosition(glm::vec3 pos)
{
    this->pos = pos;
}

void Entity::setPosition(float x, float y, float z)
{
    this->pos.x = x;
    this->pos.y = y;
    this->pos.z = z;
}

void Entity::setPosition(float x, float y)
{
    this->pos.x = x;
    this->pos.y = y;
}

void Entity::setPosX(float x)
{
    this->pos.x = x;
}

void Entity::setPosY(float y)
{
    this->pos.y = y;
}

void Entity::setZIndex(float z)
{
    this->pos.z = z;
}

void Entity::setRotate(glm::vec3 rotate)
{
    this->rotate = rotate;
}

void Entity::setRotate(float x, float y, float z)
{
    this->rotate.x = x;
    this->rotate.y = y;
    this->rotate.z = z;
}

void Entity::setRotate(float x, float y)
{
    this->rotate.x = x;
    this->rotate.y = y;
}

void Entity::setRotX(float x)
{
    this->rotate.x = x;
}

void Entity::setRotY(float y)
{
    this->rotate.y = y;
}

void Entity::setRotZ(float z)
{
    this->rotate.z = z;
}

void Entity::setScale(glm::vec3 scale)
{
    this->scale = scale;
}

void Entity::setWidth(float w)
{
    this->scale.x = w;
}

void Entity::setHeight(float h)
{
    this->scale.y = h;
}

void Entity::setScaleZ(float z)
{
    this->scale.z = z;
}

glm::vec3 Entity::getPosition()
{
    return this->pos;
}

glm::vec3 Entity::getRotation()
{
    return this->rotate;
}

glm::vec3 Entity::getScale()
{
    return this->scale;
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
    
    // Daha büyük buffer - hem arka plan hem metin için
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 6 * 2048, nullptr, GL_DYNAMIC_DRAW);

    // Vertex attribute'ları
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
    if (!shaderProgram || !fontTexture)
        return;

    glUseProgram(shaderProgram);

    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f);
    GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

    GLuint texLoc = glGetUniformLocation(shaderProgram, "textTexture");
    glUniform1i(texLoc, 0);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    if (backgroundColor.a > 0.0f)
    {
        float x = pos.x;
        float y = pos.y;
        float w = scale.x;
        float h = scale.y;

        std::vector<Vertex> bgVerts = {
            {{x,     y},     {0.0f, 0.0f}, color, backgroundColor},
            {{x+w,   y},     {1.0f, 0.0f}, color, backgroundColor},
            {{x+w,   y+h},   {1.0f, 1.0f}, color, backgroundColor},

            {{x,     y},     {0.0f, 0.0f}, color, backgroundColor},
            {{x+w,   y+h},   {1.0f, 1.0f}, color, backgroundColor},
            {{x,     y+h},   {0.0f, 1.0f}, color, backgroundColor}
        };

        glBufferSubData(GL_ARRAY_BUFFER, 0, bgVerts.size() * sizeof(Vertex), bgVerts.data());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        
        glDrawArrays(GL_TRIANGLES, 0, bgVerts.size());
    }

    if (!text.empty())
    {
        std::vector<Vertex> textVerts;
        float penX = pos.x + 10.0f;
        float penY = pos.y + (scale.y / 2) + (fontSize / 3); 

        for (char c : text)
        {
            if (c >= 32 && c < 128)
            {
                stbtt_aligned_quad q;
                stbtt_GetBakedQuad(cdata, 512, 512, c - 32, &penX, &penY, &q, 1);

                textVerts.push_back({{q.x0, q.y0}, {q.s0, q.t0}, color});
                textVerts.push_back({{q.x1, q.y0}, {q.s1, q.t0}, color});
                textVerts.push_back({{q.x1, q.y1}, {q.s1, q.t1}, color});

                textVerts.push_back({{q.x0, q.y0}, {q.s0, q.t0}, color});
                textVerts.push_back({{q.x1, q.y1}, {q.s1, q.t1}, color});
                textVerts.push_back({{q.x0, q.y1}, {q.s0, q.t1}, color});
            }
        }

        if (!textVerts.empty())
        {
            glBufferSubData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), 
                           textVerts.size() * sizeof(Vertex), textVerts.data());

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, fontTexture);
            
            glDepthFunc(GL_LEQUAL);
            
            glDrawArrays(GL_TRIANGLES, 6, textVerts.size());
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
    glDisable(GL_DEPTH_TEST);
}

void Entity::update()
{
}

void Entity::setText(const std::string& newText)
{
    text = newText;
}

void Entity::setColor(const glm::vec4& newColor)
{
    color = newColor;
}

void Entity::setFontSize(float size)
{
    fontSize = size;
    loadFont(fontPath, size);
}

void Entity::loadFont(const std::string& path, float size)
{
    fontSize = size;
    
    QFile file(QString::fromStdString(path));
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning("Font Cannot Open!");
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

    // Bitmap'i temizle
    std::fill(bitmap.begin(), bitmap.end(), 0);

    int result = stbtt_BakeFontBitmap(reinterpret_cast<const unsigned char *>(fontData.data()), 0, size,
                         bitmap.data(), bitmapWidth, bitmapHeight, 32, 96, cdata);
    
    if (result <= 0) {
        qWarning("Font baking failed! Result: %d", result);
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
    
    if (shaderProgram) glDeleteProgram(shaderProgram);
    
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        qWarning("Shader program linking failed: %s", infoLog);
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

GLuint Entity::compileShader(GLenum type, const char* source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        qWarning("Shader compilation failed: %s", infoLog);
    }
    
    return shader;
}