#include "Engine/Renderer/OpenGL/Entities/Elements/Text.hpp"

Text::Text() : 
    text("Hello World"),
    color(0.0f, 0.0f, 1.0f, 1.0f),
    fontSize(24.0f),
    fontWeight(240.0f),
    fontTexture(0),
    shaderProgram(0)
{
    fontPath = ":/fonts/nano.ttf";
}

Text::~Text()
{
    if (fontTexture) glDeleteTextures(1, &fontTexture);
    if (shaderProgram) glDeleteProgram(shaderProgram);
}

void Text::start()
{
    initializeOpenGLFunctions();
    
    loadFont(fontPath, fontSize);
    compileShaders();
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 6 * 1024, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Text::draw()
{
    if (text.empty() || !shaderProgram) return;

    std::vector<Vertex> vrtcs;
    float x = getPosition().x;
    float y = getPosition().y;

    float scaleX = 1.0f / 512.0f;
    float scaleY = -1.0f / 512.0f;

    for (char c : text)
    {
        if (c >= 32 && c < 128)
        {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(cdata, 512, 512, c - 32, &x, &y, &q, 1);

            vrtcs.push_back({{q.x0 * scaleX, q.y0 * scaleY}, {q.s0, q.t0}, color});
            vrtcs.push_back({{q.x1 * scaleX, q.y0 * scaleY}, {q.s1, q.t0}, color});
            vrtcs.push_back({{q.x1 * scaleX, q.y1 * scaleY}, {q.s1, q.t1}, color});

            vrtcs.push_back({{q.x0 * scaleX, q.y0 * scaleY}, {q.s0, q.t0}, color});
            vrtcs.push_back({{q.x1 * scaleX, q.y1 * scaleY}, {q.s1, q.t1}, color});
            vrtcs.push_back({{q.x0 * scaleX, q.y1 * scaleY}, {q.s0, q.t1}, color});
        }
    }

    glUseProgram(shaderProgram);
    
    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
    GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vrtcs.size() * sizeof(Vertex), vrtcs.data());

    glDrawArrays(GL_TRIANGLES, 0, vrtcs.size());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Text::update()
{
}

void Text::setText(const std::string& newText)
{
    text = newText;
}

void Text::setColor(const glm::vec4& newColor)
{
    color = newColor;
}

void Text::setFontSize(float size)
{
    fontSize = size;
    loadFont(fontPath, size);
}

void Text::loadFont(const std::string& path, float size)
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
    std::vector<unsigned char> bitmap(bitmapWidth * bitmapHeight);

    stbtt_fontinfo font;
    if (!stbtt_InitFont(&font, reinterpret_cast<const unsigned char *>(fontData.data()), 0))
    {
        qWarning("Failed to initialize font");
        return;
    }

    float scale = stbtt_ScaleForPixelHeight(&font, size);

    int result = stbtt_BakeFontBitmap(reinterpret_cast<const unsigned char *>(fontData.data()), 0, size,
                         bitmap.data(), bitmapWidth, bitmapHeight, 32, 96, cdata);
    
    if (result <= 0) {
        qWarning("Font baking failed!");
        return;
    }

    if (fontTexture) glDeleteTextures(1, &fontTexture);
    
    glGenTextures(1, &fontTexture);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmapWidth, bitmapHeight, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Text::compileShaders()
{
    const char* vertexShaderSource = R"(
        #version 450 core
        layout(location = 0) in vec2 aPos;
        layout(location = 1) in vec2 aUV;
        layout(location = 2) in vec4 aColor;
        
        out vec2 TexCoord;
        out vec4 FragColor;
        
        uniform mat4 projection;
        
        void main()
        {
            gl_Position = projection * vec4(aPos, 0.0, 1.0);
            TexCoord = aUV;
            FragColor = aColor;
        }
    )";
    
    const char* fragmentShaderSource = R"(
        #version 450 core
        in vec2 TexCoord;
        in vec4 FragColor;
        out vec4 FragColorOut;
        
        uniform sampler2D textTexture;
        
        void main()
        {
            float alpha = texture(textTexture, TexCoord).r;
            FragColorOut = vec4(FragColor.rgb, FragColor.a * alpha);
        }
    )";
    
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

GLuint Text::compileShader(GLenum type, const char* source)
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