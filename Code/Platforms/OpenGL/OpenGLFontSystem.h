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
#include "Components/UIComponents.h"
#include "Graphics/FontSystem.h"

class UIText;

typedef std::map<GLchar, Character> FontFace;

class OpenGLFontSystem : public FontSystem
{
public:
    virtual int Initialize() override;
    virtual void RenderText(const std::string& text, const glm::vec2& position, float scale, const glm::vec4& color, const glm::vec2& viewportSize, UIFlag flag) override;
    virtual void LoadFont(Font& font) override;

private:
    std::map<GLchar, Character> Characters;

    std::unordered_map<std::string, FontFace> fontFaceMap;

    unsigned int VAO, VBO;
    OpenGLShader shader;
};