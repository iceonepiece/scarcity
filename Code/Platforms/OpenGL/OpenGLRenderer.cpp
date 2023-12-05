#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGLRenderer.h"
#include "OpenGLTexture.h"
#include "OpenGLFramebuffer.h"
#include "Graphics/Image.h"

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
    
    // setup circle
    float right = 0.5;
    float bottom = -0.5;
    float left = -0.5;
    float top = 0.5;
    float quad[20] = {
        right, bottom, 0, 1.0, -1.0,
        right, top, 0, 1.0, 1.0,
        left, top, 0, -1.0, 1.0,
        left, bottom, 0, -1.0, -1.0,
    };

    glGenBuffers(1, &m_circleVBO);
    glGenVertexArrays(1, &m_circleVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_circleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 20, quad, GL_STATIC_DRAW);

    glBindVertexArray(m_circleVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // clear
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_basicShader.Compile("Shaders/basic.vert", "Shaders/basic.frag");
    m_spriteShader.Compile("Shaders/texture.vert", "Shaders/texture.frag");
    m_uiShader.Compile("Shaders/ui.vert", "Shaders/ui.frag");
    m_circleShader.Compile("Shaders/circle.vert", "Shaders/circle.frag");

    m_fontSystem.Init();
}

void OpenGLRenderer::Draw(Sprite& sprite, const glm::mat4& modelMatrix)
{
    m_spriteShader.Use();

    m_spriteShader.SetMatrix4("model", modelMatrix);
    //m_spriteShader.SetMatrix4("view", m_camera->GetViewMatrix());
    //m_spriteShader.SetMatrix4("projection", m_camera->GetProjectionMatrix());

    m_spriteShader.SetMatrix4("view", m_viewMatrix);
    m_spriteShader.SetMatrix4("projection", m_projectionMatrix);

    glActiveTexture(GL_TEXTURE0);
    OpenGLTexture *texture = static_cast<OpenGLTexture*>(sprite.GetTexture());
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

void OpenGLRenderer::DrawSprite(Sprite& sprite, const glm::vec2& position, const glm::vec2& scale, float angle, glm::vec4 color)
{
    m_spriteShader.Use();

    glm::vec2 ratio = sprite.GetRatio();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
    model = glm::rotate(model, angle, glm::vec3(0, 0, 1));
    model = glm::scale(model, glm::vec3(scale.x * ratio.x, scale.y * ratio.y, 0.0f));

    m_spriteShader.SetMatrix4("model", model);
    m_spriteShader.SetMatrix4("view", m_viewMatrix);
    m_spriteShader.SetMatrix4("projection", m_projectionMatrix);

    glActiveTexture(GL_TEXTURE0);

    Texture* texture = sprite.GetTexture();

    if (texture == nullptr)
        return;

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

    m_basicShader.SetMatrix4("mvp", m_viewProjectionMatrix);
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

void OpenGLRenderer::DrawLines(float lines[], int n, const glm::vec4& color)
{
    m_basicShader.Use();

    glm::mat4 projection = glm::mat4(1.0f);
    //glm::vec2 screenSize = RendererAPI::GetScreenSize();
    //projection = glm::perspective(glm::radians(45.0f), screenSize.x / screenSize.y, 0.1f, 100.0f);
    projection = m_camera->GetProjectionMatrix();

    //m_basicShader.SetMatrix4("model", glm::mat4(1));

    glm::mat4 view = glm::mat4(1.0f);
    if (m_camera != nullptr)
        view = m_camera->GetViewMatrix();

    m_basicShader.SetMatrix4("mvp", projection * view);
    m_basicShader.SetVector4f("color", color);
    
    glBindVertexArray(m_lineVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * n * 2, lines, GL_STATIC_DRAW);

    glDrawArrays(GL_LINES, 0, n);
}

void OpenGLRenderer::DrawQuad(const glm::vec2& position, const glm::vec2& scale, float angle, glm::vec4 color)
{
    m_basicShader.Use();

    //glm::mat4 projection = glm::mat4(1.0f);
    //glm::vec2 screenSize = RendererAPI::GetScreenSize();
    //projection = glm::perspective(glm::radians(45.0f), screenSize.x / screenSize.y, 0.1f, 100.0f);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
    model = glm::rotate(model, angle, glm::vec3(0, 0, 1));
    model = glm::scale(model, glm::vec3(scale.x, scale.y, 0.0f));

    m_basicShader.SetMatrix4("mvp", m_camera->GetProjectionMatrix() * m_camera->GetViewMatrix() * model);
    m_basicShader.SetVector4f("color", color);

    glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void OpenGLRenderer::DrawQuad2D(const glm::vec2& position, const glm::vec2& scale, float angle, glm::vec4 color)
{
    m_basicShader.Use();

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
    model = glm::rotate(model, angle, glm::vec3(0, 0, 1));
    model = glm::scale(model, glm::vec3(scale.x, scale.y, 0.0f));

    m_basicShader.SetMatrix4("mvp", m_viewProjectionMatrix * model);
    m_basicShader.SetVector4f("color", color);

    glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void OpenGLRenderer::DrawQuad2D(const Quad2D& quad)
{
    m_basicShader.Use();

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(quad.position.x, quad.position.y, 0.0f));
    model = glm::rotate(model, quad.angle, glm::vec3(0, 0, 1));
    model = glm::scale(model, glm::vec3(quad.scale.x, quad.scale.y, 0.0f));

    m_basicShader.SetMatrix4("mvp", m_viewProjectionMatrix * model);
    m_basicShader.SetVector4f("color", quad.color);

    glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void OpenGLRenderer::DrawRect(b2Body* body, const Camera& camera)
{
    b2Vec2 position = body->GetPosition();

 
    glm::mat4 model = glm::mat4(1.0f);

    float posX = position.x;
    float posY = position.y;
    float width = 1;
    float height = 1;

    b2Fixture* fixture = body->GetFixtureList();
    if (fixture)
    {
        b2PolygonShape* shape = (b2PolygonShape*)fixture->GetShape();
        width = shape->m_vertices[1].x - shape->m_vertices[0].x;
        height = shape->m_vertices[2].y - shape->m_vertices[0].y;

        posX += shape->m_centroid.x;
        posY += shape->m_centroid.y;
    }

    model = glm::translate(model, glm::vec3(posX, posY, 0.0f));
    model = glm::rotate(model, body->GetAngle(), glm::vec3(0, 0, 1));
    model = glm::scale(model, glm::vec3(width, height, 0.0f));

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

glm::vec2 RotatedPosition(const glm::vec2& offset, const glm::vec2& local, float angle)
{
    glm::vec2 renderPosition { offset.x, offset.y };
    glm::vec2 rotated = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 0, 1)) * glm::vec4(local.x, local.y, 0.0f, 1.0f);

    return renderPosition + rotated;
}

void OpenGLRenderer::DrawRect(const glm::vec2& position, const glm::vec2& scale, float angle, glm::vec4 color, float thickness)
{
    glm::vec2 vScale { thickness, scale.y };
    glm::vec2 hScale { scale.x, thickness };

    float offsetX = scale.x / 2 - thickness / 2;
    float offsetY = scale.y / 2 - thickness / 2;

    DrawQuad2D(RotatedPosition(position, { 0, offsetY }, angle), hScale, angle, color);
    DrawQuad2D(RotatedPosition(position, { 0, -offsetY }, angle), hScale, angle, color);
    DrawQuad2D(RotatedPosition(position, { offsetX, 0 }, angle), vScale, angle, color);
    DrawQuad2D(RotatedPosition(position, { -offsetX, 0 }, angle), vScale, angle, color);
}

void OpenGLRenderer::DrawCircle(const glm::vec2& position, float radius)
{
    m_circleShader.Use();

    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
    model = glm::scale(model, glm::vec3(radius * 2, radius * 2, 0.0f));

    m_circleShader.SetMatrix4("model", model);
    m_circleShader.SetMatrix4("view", m_camera->GetViewMatrix());
    m_circleShader.SetMatrix4("projection", m_camera->GetProjectionMatrix(CameraType::Orthographic));

    glBindVertexArray(m_circleVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void OpenGLRenderer::DrawCircle2D(const Circle2D& circle, float thickness)
{
    m_circleShader.Use();

    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(circle.position.x, circle.position.y, 0.0f));
    model = glm::scale(model, glm::vec3(circle.radius * 2 * circle.scale.x, circle.radius * 2 * circle.scale.y, 0.0f));

    m_circleShader.SetMatrix4("model", model);
    //m_circleShader.SetMatrix4("view", m_camera->GetViewMatrix());
    //m_circleShader.SetMatrix4("projection", m_camera->GetProjectionMatrix(CameraType::Orthographic));
    m_circleShader.SetMatrix4("view", m_viewMatrix);
    m_circleShader.SetMatrix4("projection", m_projectionMatrix);
    m_circleShader.SetVector4f("color", circle.color);
    m_circleShader.SetFloat("minRadius", 1.0f - thickness);

    glBindVertexArray(m_circleVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void OpenGLRenderer::DrawQuadUI(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color)
{
    m_uiShader.Use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position.x, m_screenSize.y - position.y, 0.0f));
    model = glm::scale(model, glm::vec3(scale.x, scale.y, 0.0f));

    glm::mat4 projection = glm::ortho(0.0f, m_screenSize.x, 0.0f, m_screenSize.y);

    m_uiShader.SetMatrix4("model", model);
    m_uiShader.SetMatrix4("projection", projection);
    m_uiShader.SetVector4f("color", color);

    glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void OpenGLRenderer::DrawText(const std::string& text, const glm::vec2& position, float scale, const glm::vec4& color)
{
    m_fontSystem.RenderText(text, position, scale, color, m_screenSize);
}

void OpenGLRenderer::Clear(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::PreRender(bool font)
{
    if (font)
        glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLRenderer::PostRender(bool font)
{
    if (font)
        glDisable(GL_CULL_FACE);

    glDisable(GL_BLEND);
}

std::unique_ptr<Framebuffer> OpenGLRenderer::CreateFramebuffer()
{
    return std::make_unique<OpenGLFramebuffer>();
}