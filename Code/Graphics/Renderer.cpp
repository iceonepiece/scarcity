#include "Renderer.h"
#include <iostream>
#include "Platforms\OpenGL\OpenGLRenderer.h"

glm::vec4 Renderer::s_quadVertices[4]
{
    { -0.5f, -0.5f, 0.0f, 1.0f },
    {  0.5f, -0.5f, 0.0f, 1.0f },
    {  0.5f,  0.5f, 0.0f, 1.0f },
    { -0.5f,  0.5f, 0.0f, 1.0f },
};

void Renderer::Initialize()
{
    m_quadVertexArray = VertexArray::Create();

    m_quadVertexBuffer = VertexBuffer::Create(MAX_VERTICES * sizeof(QuadVertex));
    m_quadVertexBuffer->SetLayout({
        { ShaderDataType::Float3, "a_Position"     },
        { ShaderDataType::Float4, "a_Color"        },
        { ShaderDataType::Float2, "a_TexCoord"     },
        { ShaderDataType::Float,  "a_TexIndex"     },
    });
    m_quadVertexArray->AddVertexBuffer(m_quadVertexBuffer);
    m_quadVertexBufferBase = new QuadVertex[MAX_VERTICES];

    uint32_t* quadIndices = new uint32_t[MAX_INDICES];

    uint32_t offset = 0;
    for (uint32_t i = 0; i < MAX_INDICES; i += 6)
    {
        quadIndices[i + 0] = offset + 0;
        quadIndices[i + 1] = offset + 1;
        quadIndices[i + 2] = offset + 2;

        quadIndices[i + 3] = offset + 2;
        quadIndices[i + 4] = offset + 3;
        quadIndices[i + 5] = offset + 0;

        offset += 4;
    }

    std::shared_ptr<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, MAX_INDICES);
    m_quadVertexArray->SetIndexBuffer(quadIB);
    delete[] quadIndices;

    m_fontSystem = std::make_unique<OpenGLFontSystem>();
    m_fontSystem->Initialize();

}

Renderer::~Renderer()
{
    delete[] m_quadVertexBufferBase;
};

void Renderer::DrawText(const std::string& text, const glm::vec2& position, float scale, const glm::vec4& color, const std::string& fontName)
{
    //m_drawTextCommands.push_back({ text, position, scale, color, fontName });
    m_fontSystem->SetFont(fontName);
    m_fontSystem->RenderText(text, position, scale, color, m_screenSize);
}

void Renderer::DrawSprite(Sprite& sprite, const glm::vec2& position, const glm::vec2& scale, float angle, glm::vec4 color)
{
    constexpr size_t quadVertexCount = 4;
    glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

    if (m_quadIndexCount >= MAX_INDICES)
        NextBatch();

	Texture* texture = sprite.GetTexture();

    float textureIndex = 0.0f;
    for (uint32_t i = 1; i < m_textureIndex; i++)
    {
        if (*m_textures[i] == *texture)
        {
            textureIndex = (float)i;
            break;
        }
    }

    if (textureIndex == 0.0f)
    {
        if (m_textureIndex >= MAX_TEXTURES)
            NextBatch();

        textureIndex = (float)m_textureIndex;
        m_textures[m_textureIndex] = texture;
        m_textureIndex++;
    }

    glm::vec2 ratio { 1.0f, 1.0f };

    if (texture != nullptr)
    {
        ratio = sprite.GetRatio();

        float left = sprite.GetLeft();
        float right = sprite.GetRight();
        float bottom = sprite.GetBottom();
        float top = sprite.GetTop();

		textureCoords[0] = { left, bottom };
		textureCoords[1] = { right, bottom };
		textureCoords[2] = { right, top };
		textureCoords[3] = { left, top };
    }


    /*
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.0f })
        * glm::rotate(glm::mat4(1.0f), angle, { 0.0f, 0.0f, 1.0f })
        * glm::scale(glm::mat4(1.0f), { scale.x * ratio.x, scale.y * ratio.y, 1.0f });
     */

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.0f })
        * glm::mat4(glm::quat({ 0.0f, 0.0f, angle }))
        * glm::scale(glm::mat4(1.0f), { scale.x * ratio.x, scale.y * ratio.y, 1.0f });


    for (size_t i = 0; i < quadVertexCount; i++)
    {
        m_quadVertexBufferPtr->position = transform * s_quadVertices[i];
        m_quadVertexBufferPtr->color = color;
        m_quadVertexBufferPtr->texCoord = textureCoords[i];
        m_quadVertexBufferPtr->texIndex = textureIndex;
        m_quadVertexBufferPtr++;
    }

    m_quadIndexCount += 6;
}

void Renderer::BeginFrame()
{
    StartBatch();
}

void Renderer::EndFrame()
{
	Flush();
}

void Renderer::Flush()
{
    if (m_quadIndexCount)
    {
        m_quadShader->Use();
        m_quadShader->SetMatrix4("u_ViewProjection", m_viewProjectionMatrix);

        uint32_t dataSize = (uint32_t)((uint8_t*)m_quadVertexBufferPtr - (uint8_t*)m_quadVertexBufferBase);
        m_quadVertexBuffer->SetData(m_quadVertexBufferBase, dataSize);

        for (uint32_t i = 0; i < m_textureIndex; i++)
        {
            m_quadShader->SetInt("texture" + std::to_string(i), i);
            m_textures[i]->Bind(i);
        }
        
        DrawIndexed(m_quadVertexArray, m_quadIndexCount);
    }

    for (auto& drawCommand : m_drawTextCommands)
    {
        m_fontSystem->SetFont(drawCommand.fontName);
        m_fontSystem->RenderText(drawCommand.text, drawCommand.position, drawCommand.size, drawCommand.color, m_screenSize);
    }
    m_drawTextCommands.clear();
}

void Renderer::StartBatch()
{
    m_quadIndexCount = 0;
	m_quadVertexBufferPtr = m_quadVertexBufferBase;
    m_textureIndex = 0;
}

void Renderer::NextBatch()
{
    Flush();
    StartBatch();
}

void Renderer::SetCamera(const Camera& camera)
{
    m_viewProjectionMatrix = camera.GetProjectionMatrix() * camera.GetViewMatrix();
}

void Renderer::SetCamera(CameraComponent cameraComponent)
{
    m_cameraComponent = cameraComponent;
}

std::unique_ptr<Renderer> Renderer::Create()
{
    return std::make_unique<OpenGLRenderer>();
}






