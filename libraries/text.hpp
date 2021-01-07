class Text {
    public: 
        std::string text;
        float x;
        float y;
        float scale;
        glm::vec3 color;
        Font* font;

        Text (
            std::string text,
            float x,
            float y,
            float scale,
            glm::vec3 color,
            Font* font
        );
};

Text::Text(std::string text,
            float x,
            float y,
            float scale,
            glm::vec3 color,
            Font* font) {
    this->text = text;
    this->x = x;
    this->y = y;
    this->scale = scale;
    this->color = color;
    this->font = font;
};

void drawText (Text text) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(text.font->id);
    glUniform3f(glGetUniformLocation(text.font->id, "textColor"), text.color.x, text.color.y, text.color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(text.font->idVAO);

        // iterate through all characters
    std::string::const_iterator c;
    for (c = text.text.begin(); c != text.text.end(); c++)
    {
        Character ch = text.font->textCharacters[*c];

        float xpos = text.x + ch.Bearing.x * text.scale;
        float ypos = text.y - (ch.Size.y - ch.Bearing.y) * text.scale;

        float w = ch.Size.x * text.scale;
        float h = ch.Size.y * text.scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, text.font->idVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        text.x += (ch.Advance >> 6) * text.scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_BLEND);
}