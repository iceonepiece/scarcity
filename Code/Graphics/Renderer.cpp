#include "Renderer.h"
#include "../Platforms/OpenGLTexture.h"
#include <iostream>

unsigned int Renderer::m_VBO = 0;
unsigned int Renderer::m_VAO = 0;
glm::vec2 Renderer::s_screenSize;
glm::vec2 Renderer::s_screenOffset;
glm::vec2 Renderer::s_defaultScreenSize = glm::vec2(1280, 720);

Shader Renderer::s_basicShader;
Shader Renderer::s_uiShader;
Shader Renderer::s_circleShader;
Shader Renderer::s_textureShader;

void Renderer::SetCamera(Camera *camera)
{
    m_camera = camera;
}

void Renderer::SetScreenSize(int width, int height, int xOffset, int yOffset)
{
    s_screenSize.x = width;
    s_screenSize.y = height;
    s_screenOffset.x = xOffset;
    s_screenOffset.y = yOffset;
}

glm::vec2 Renderer::GetScreenSize()
{
    return glm::vec2(s_screenSize.x, s_screenSize.y);
}

float Renderer::GetScreenSizePercentage()
{
    return s_screenSize.x / s_defaultScreenSize.x;
}

void Renderer::Init()
{
    float vertices[] = {
         0.5f,  0.5f,   1.0f,   1.0f,
         0.5f, -0.5f,   1.0f,   0.0f,
        -0.5f,  0.5f,   0.0f,   1.0f,
         0.5f, -0.5f,   1.0f,   0.0f,
        -0.5f, -0.5f,   0.0f,   0.0f,
        -0.5f,  0.5f,   0.0f,   1.0f
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

    s_basicShader.Compile("Code/Shaders/basic.vert", "Code/Shaders/basic.frag");
    s_uiShader.Compile("Code/Shaders/ui.vert", "Code/Shaders/ui.frag");
    s_circleShader.Compile("Code/Shaders/circle.vert", "Code/Shaders/circle.frag");
    s_textureShader.Compile("Code/Shaders/texture.vert", "Code/Shaders/texture.frag");
}

/*
void Renderer::DrawPolygon(b2Body* body, Camera& camera)
{
    s_basicShader.Use();

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

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, camera.GetPosition());

    //glm::vec2 screenSize = camera.GetScreenSize();

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), s_screenSize.x / s_screenSize.y, 0.1f, 100.0f);

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
*/

void Renderer::DrawQuad(b2Body* body, Camera& camera)
{
    s_basicShader.Use();

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

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, camera.GetPosition());

   //glm::vec2 screenSize = camera.GetScreenSize();

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), s_screenSize.x / s_screenSize.y, 0.1f, 100.0f);

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

void Renderer::DrawCircle(b2Body* body, glm::vec4 color, Camera& camera)
{

    b2Fixture* fixture = body->GetFixtureList();

    if (!fixture)
        return;

    s_circleShader.Use();

    b2Vec2 vecPos = body->GetPosition();
    glm::vec2 position{ vecPos.x, vecPos.y };

    b2CircleShape* shape = (b2CircleShape*)fixture->GetShape();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
    model = glm::scale(model, glm::vec3(shape->m_radius * 2, shape->m_radius * 2, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, camera.GetPosition());

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), s_screenSize.x / s_screenSize.y, 0.1f, 100.0f);

    glm::vec4 pos = projection * view * model * glm::vec4(0, 0, 0, 1);
    glm::vec2 ndcPos = glm::vec2(pos.x / pos.w, pos.y / pos.w);
    glm::vec2 pixelPos = glm::vec2((ndcPos.x * 0.5 + 0.5) * s_screenSize.x, (ndcPos.y * 0.5 + 0.5) * s_screenSize.y);

    glm::vec4 leftPos = projection * view * model * glm::vec4(-0.5, 0, 0, 1);
    glm::vec2 ndcLeftPos = glm::vec2(leftPos.x / leftPos.w, leftPos.y / leftPos.w);
    glm::vec2 pixelLeftPos = glm::vec2((ndcLeftPos.x * 0.5 + 0.5) * s_screenSize.x, (ndcLeftPos.y * 0.5 + 0.5) * s_screenSize.y);

    float pixelRadius = pixelPos.x - pixelLeftPos.x;

    s_circleShader.SetMatrix4("model", model);
    s_circleShader.SetMatrix4("view", view);
    s_circleShader.SetMatrix4("projection", projection);
    s_circleShader.SetVector4f("color", color);
    s_circleShader.SetVector3f("resolution", glm::vec3(s_screenSize.x, s_screenSize.y, 0));
    s_circleShader.SetVector3f("pixelPos", glm::vec3(pixelPos.x + s_screenOffset.x, pixelPos.y + s_screenOffset.y, 0));
    s_circleShader.SetFloat("pixelRadius", pixelRadius);

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::DrawCircle(glm::vec2 position, float radius, glm::vec4 color, Camera& camera)
{
    s_circleShader.Use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
    model = glm::scale(model, glm::vec3(radius, radius, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, camera.GetPosition());

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), s_screenSize.x / s_screenSize.y, 0.1f, 100.0f);

    glm::vec4 pos = projection * view * model * glm::vec4(0, 0, 0, 1);
    glm::vec2 ndcPos = glm::vec2(pos.x / pos.w, pos.y / pos.w);
    glm::vec2 pixelPos = glm::vec2((ndcPos.x * 0.5 + 0.5) * s_screenSize.x, (ndcPos.y * 0.5 + 0.5) * s_screenSize.y);
    
    glm::vec4 leftPos = projection * view * model * glm::vec4(-0.5, 0, 0, 1);
    glm::vec2 ndcLeftPos = glm::vec2(leftPos.x / leftPos.w, leftPos.y / leftPos.w);
    glm::vec2 pixelLeftPos = glm::vec2((ndcLeftPos.x * 0.5 + 0.5) * s_screenSize.x, (ndcLeftPos.y * 0.5 + 0.5) * s_screenSize.y);

    float pixelRadius = pixelPos.x - pixelLeftPos.x;

    s_circleShader.SetMatrix4("model", model);
    s_circleShader.SetMatrix4("view", view);
    s_circleShader.SetMatrix4("projection", projection);
    s_circleShader.SetVector4f("color", color);
    s_circleShader.SetVector3f("resolution", glm::vec3(s_screenSize.x, s_screenSize.y, 0));
    s_circleShader.SetVector3f("pixelPos", glm::vec3(pixelPos.x, pixelPos.y, 0));
    s_circleShader.SetFloat("pixelRadius", pixelRadius);

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::DrawQuadUI(glm::vec2 position, glm::vec2 scale, glm::vec4 color, UIAlignment alignment)
{
    s_uiShader.Use();

    float x = position.x;
    float y = position.y;

    if (alignment == UIAlignment::CENTER)
    {
        x = s_screenSize.x / 2;
        y = s_screenSize.y / 2;
    }

    glm::vec2 realScale = scale * Renderer::GetScreenSizePercentage();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x, y, 0.0f));
    model = glm::scale(model, glm::vec3(realScale.x, realScale.y, 0.0f));

    glm::mat4 projection = glm::ortho(0.0f, s_screenSize.x, 0.0f, s_screenSize.y);

    s_uiShader.SetMatrix4("model", model);
    s_uiShader.SetMatrix4("projection", projection);
    s_uiShader.SetVector4f("color", color);

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::DrawSprite(Sprite& sprite, glm::vec2 position, glm::vec2 scale, Camera& camera)
{
    s_textureShader.Use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));

    glm::vec2 spriteRatio = sprite.GetRatio();
    model = glm::scale(model, glm::vec3(scale.x * spriteRatio.x, scale.y * spriteRatio.y, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, camera.GetPosition());

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), s_screenSize.x / s_screenSize.y, 0.1f, 100.0f);

    s_textureShader.SetMatrix4("model", model);
    s_textureShader.SetMatrix4("view", view);
    s_textureShader.SetMatrix4("projection", projection);

    glActiveTexture(GL_TEXTURE0);
    OpenGLTexture* texture = static_cast<OpenGLTexture*>(sprite.GetTexture());
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