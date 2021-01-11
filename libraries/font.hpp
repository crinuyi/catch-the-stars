#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>
#include <iostream>

// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    long int Advance;       // Horizontal offset to advance to next glyph
};

class Font {
    public: 
        GLuint id;
        GLuint idVAO;
        GLuint idVBO;
        std::map<GLchar, Character> textCharacters;
        const char* path;
        int size = 100;

        Font(
            char const* path
        );
};

Font::Font(char const* path) {
    this->path = path;

    this->id = glCreateProgram();
    glAttachShader(this->id, LoadShader(GL_VERTEX_SHADER, "shaders/textVertex.glsl"));
    glAttachShader(this->id, LoadShader(GL_FRAGMENT_SHADER, "shaders/textFragment.glsl"));
    LinkAndValidateProgram(this->id);

    int Window_Width = glutGet(GLUT_WINDOW_WIDTH);
    int Window_Height = glutGet(GLUT_WINDOW_HEIGHT);

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Window_Width), 0.0f, static_cast<float>(Window_Height));
    glUseProgram(this->id);
    glUniformMatrix4fv(glGetUniformLocation(this->id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    FT_Face face;
    if (FT_New_Face(ft, path, 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    FT_Set_Pixel_Sizes(face, 0, size);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // load first 128 characters of ASCII set
    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        textCharacters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &(this->idVAO));
    glBindVertexArray(this->idVAO);
    glGenBuffers(1, &(this->idVBO));
    glBindBuffer(GL_ARRAY_BUFFER, this->idVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
};