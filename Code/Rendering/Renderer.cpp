#include "Renderer.h"
#include <iostream>

unsigned int Renderer::m_VBO = 0;
unsigned int Renderer::m_VAO = 0;

Shader Renderer::s_basicShader;
Shader Renderer::s_uiShader;

void Renderer::Init()
{
    float vertices[] = {
        0.5f,  0.5f,
        0.5f, -0.5f,
        -0.5f,  0.5f,
        0.5f, -0.5f,
        -0.5f, -0.5f,
        -0.5f,  0.5f
    };

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(m_VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    s_basicShader.Compile("Code/Shaders/basic.vert", "Code/Shaders/basic.frag");
    s_uiShader.Compile("Code/Shaders/ui.vert", "Code/Shaders/ui.frag");
}

void Renderer::DrawQuad(b2Body* body, Camera& camera)
{
    s_basicShader.Use();

    b2Vec2 position = body->GetPosition();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));

    b2Fixture* fixture = body->GetFixtureList();
    if (fixture)
    {
        b2PolygonShape* shape = (b2PolygonShape*)fixture->GetShape();
        float width = shape->m_vertices[1].x - shape->m_vertices[0].x;
        float height = shape->m_vertices[2].y - shape->m_vertices[0].y;

        model = glm::scale(model, glm::vec3(width, height, 0.0f));
    }

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, camera.GetPosition());

    glm::vec2 screenSize = camera.GetScreenSize();
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), screenSize.x / screenSize.y, 0.1f, 100.0f);

    glm::vec4 color = glm::vec4(0.188f, 0.278f, 0.369f, 0.2f);

    if (body->GetType() == b2_dynamicBody)
        color = glm::vec4(0.941f, 0.329f, 0.329f, 1.0f);

    s_basicShader.SetMatrix4("model", model);
    s_basicShader.SetMatrix4("view", view);
    s_basicShader.SetMatrix4("projection", projection);
    s_basicShader.SetVector4f("color", color);

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::DrawQuadUI(glm::vec2 position, glm::vec2 scale, glm::vec4 color)
{
    s_uiShader.Use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
    model = glm::scale(model, glm::vec3(scale.x, scale.y, 0.0f));

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(1280), 0.0f, static_cast<float>(720));

    s_uiShader.SetMatrix4("model", model);
    s_uiShader.SetMatrix4("projection", projection);
    s_uiShader.SetVector4f("color", color);

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}