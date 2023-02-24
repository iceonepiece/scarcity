#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGLRenderer.h"
#include "RendererAPI.h"

void OpenGLRenderer::Initialize()
{
    float vertices[] = {
         0.5f,   0.5f,   1.0f,   1.0f,
         0.5f,  -0.5f,   1.0f,   0.0f,
        -0.5f,   0.5f,   0.0f,   1.0f,
         0.5f,  -0.5f,   1.0f,   0.0f,
        -0.5f,  -0.5f,   0.0f,   0.0f,
        -0.5f,   0.5f,   0.0f,   1.0f
    };

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(m_VAO);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_spriteShader.Compile("Code/Shaders/texture.vert", "Code/Shaders/texture.frag");
}

void OpenGLRenderer::Draw(Sprite& sprite, const glm::mat4& modelMatrix, Camera& camera)
{
    
    m_spriteShader.Use();

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, camera.GetPosition());
  
    glm::mat4 projection = glm::mat4(1.0f);
    glm::vec2 screenSize = RendererAPI::GetScreenSize();
    projection = glm::perspective(glm::radians(45.0f), screenSize.x / screenSize.y, 0.1f, 100.0f);

    m_spriteShader.SetMatrix4("model", modelMatrix);
    m_spriteShader.SetMatrix4("view", view);
    m_spriteShader.SetMatrix4("projection", projection);

    glActiveTexture(GL_TEXTURE0);
    Texture* texture = sprite.GetTexture();
    texture->Bind();

    float left = sprite.GetLeft();
    float right = sprite.GetRight();
    float bottom = sprite.GetBottom();
    float top = sprite.GetTop();

    float vertices[] = {
        0.5f,  0.5f,   right,   top,
        0.5f, -0.5f,   right,  bottom,
       -0.5f,  0.5f,   left,   top,
        0.5f, -0.5f,   right,   bottom,
       -0.5f, -0.5f,   left,   bottom,
       -0.5f,  0.5f,   left,   top
    };

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}