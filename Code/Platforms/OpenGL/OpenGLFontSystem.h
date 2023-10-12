#pragma once

#include <iostream>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Platforms/OpenGL/OpenGLShader.h"

class UIText;

struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

class OpenGLFontSystem
{
public:
	int Init();

    void RenderText(UIText *uiText, const glm::vec2& viewportSize);

private:
    const uint8_t FONT_PIXEL_HEIGHT = 128;
    std::map<GLchar, Character> Characters;
    unsigned int VAO, VBO;
    OpenGLShader shader;

};