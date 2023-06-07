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

    glGenVertexArrays(1, &m_quadVAO);
    glGenBuffers(1, &m_quadVBO);

    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(m_quadVAO);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // setup line
    m_lineVAO = 0;
    m_lineVBO = 0;

    glGenVertexArrays(1, &m_lineVAO);
    glGenBuffers(1, &m_lineVBO);

    glBindVertexArray(m_lineVAO);

    float line[] = {
        0.0f, 0.0f,
        0.0f, 0.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // clear
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_basicShader.Compile("Code/Shaders/basic.vert", "Code/Shaders/basic.frag");
    m_spriteShader.Compile("Code/Shaders/texture.vert", "Code/Shaders/texture.frag");
}

void OpenGLRenderer::Draw(Sprite& sprite, const glm::mat4& modelMatrix)
{

    m_spriteShader.Use();

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, m_camera->GetPosition());
  
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

    glBindVertexArray(m_quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void OpenGLRenderer::DrawLine(const glm::vec3& v1, const glm::vec3& v2, const glm::vec4& color)
{
    m_basicShader.Use();

    glm::mat4 projection = glm::mat4(1.0f);
    glm::vec2 screenSize = RendererAPI::GetScreenSize();
    projection = glm::perspective(glm::radians(45.0f), screenSize.x / screenSize.y, 0.1f, 100.0f);

    m_basicShader.SetMatrix4("model", glm::mat4(1));

    glm::mat4 view = glm::mat4(1.0f);
    if (m_camera != nullptr)
        view = m_camera->GetViewMatrix();

    m_basicShader.SetMatrix4("view", view);
    m_basicShader.SetMatrix4("projection", projection);
    m_basicShader.SetVector4f("color", color);

    glBindVertexArray(m_lineVAO);

    float line[] = {
        v1.x, v1.y,
        v2.x, v2.y
    };
    glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);

    glDrawArrays(GL_LINES, 0, 2);
}

void OpenGLRenderer::DrawRect(b2Body* body, const Camera& camera)
{
    b2Vec2 position = body->GetPosition();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
    model = glm::rotate(model, body->GetAngle(), glm::vec3(0, 0, 1));

    b2Fixture* fixture = body->GetFixtureList();
    if (fixture)
    {
        b2PolygonShape* shape = (b2PolygonShape*)fixture->GetShape();
        float width = shape->m_vertices[1].x - shape->m_vertices[0].x;
        float height = shape->m_vertices[2].y - shape->m_vertices[0].y;

        model = glm::scale(model, glm::vec3(width, height, 0.0f));
    }

    float vertices[4][3] = {
        {  0.5f,   0.5f,  0.0f },
        {  0.5f,  -0.5f,  0.0f },
        { -0.5f,  -0.5f,  0.0f },
        { -0.5f,   0.5f,  0.0f }
    };

    std::vector<glm::vec3> points;
    for (auto v : vertices)
    {
        glm::vec4 point{ v[0], v[1], v[2], 1.0f };
        points.push_back(glm::vec3(model * point));
    }

    glm::vec4 color(0.2f, 1.0f, 0.2f, 1.0f);

    DrawLine(points[0], points[1], color);
    DrawLine(points[1], points[2], color);
    DrawLine(points[2], points[3], color);
    DrawLine(points[3], points[0], color);
}

void OpenGLRenderer::DrawCircle(const glm::vec2& position, float radius)
{

}