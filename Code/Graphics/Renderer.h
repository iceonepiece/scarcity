#pragma once

#include <box2d/box2d.h>

#include "Graphics/Shader.h"
#include "Graphics/Camera.h"
#include "Graphics/Texture.h"
#include "Graphics/Buffer.h"
#include "Graphics/Framebuffer.h"
#include "Graphics/Sprite.h"
#include "Graphics/VertexArray.h"
#include "Graphics/FontSystem.h"
#include "Shapes/Shape2D.h"
#include "Components/CameraComponent.h"

struct QuadVertex
{
	glm::vec3 position;
	glm::vec4 color;
	glm::vec2 texCoord;
	float texIndex;
};

struct DrawTextCommand
{
	std::string text;
	glm::vec2 position;
	float size;
	glm::vec4 color;
	std::string fontName;
};

class Renderer
{
public:
	static const uint32_t MAX_QUADS = 1000;
	static const uint32_t MAX_VERTICES = MAX_QUADS * 4;
	static const uint32_t MAX_INDICES = MAX_QUADS * 6;
	static const uint32_t MAX_TEXTURES = 32;

	static std::unique_ptr<Renderer> Create();

	virtual ~Renderer();

	void BeginFrame();
	void EndFrame();
	void Flush();

	virtual void Initialize();
	virtual void Draw(Sprite& sprite, const glm::mat4& modelMatrix) = 0;
	virtual void DrawSprite(Sprite& sprite, const glm::vec2& position, const glm::vec2& scale, float angle = 0.0f, glm::vec4 color = glm::vec4{ 1.0f });
	virtual void DrawQuad(const glm::vec2& position, const glm::vec2& scale, float angle = 0.0f, glm::vec4 color = glm::vec4{ 1.0f }) = 0;
	virtual void DrawQuad2D(const glm::vec2& position, const glm::vec2& scale, float angle = 0.0f, glm::vec4 color = glm::vec4{ 1.0f }) = 0;
	virtual void DrawQuad2D(const Quad2D& quad) = 0;
	virtual void DrawQuadUI(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color) = 0;
	virtual void DrawLine(const glm::vec3& v1, const glm::vec3& v2, const glm::vec4& color = glm::vec4(1)) = 0;
	virtual void DrawLines(float lines[], int n, const glm::vec4& color = glm::vec4(1)) = 0;
	virtual void DrawRect(b2Body* body, const Camera& camera) = 0;
	virtual void DrawRect(const glm::vec2& position, const glm::vec2& scale, float angle = 0.0f, glm::vec4 color = glm::vec4{ 1.0f }, float thickness = 1.0f) = 0;
	virtual void DrawCircle(const glm::vec2& position, float radius) = 0;
	virtual void DrawCircle2D(const Circle2D& circle, float thickness = 1.0f) = 0;
	
	void DrawText(const std::string& text, const glm::vec2& position, float scale, const glm::vec4& color, const std::string& fontName, UIRect rect, UIFlag flags = UIFlag_None);

	virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount) = 0;

	virtual std::unique_ptr<Texture> LoadTexture(const std::string& name, const char* filename, bool alpha) = 0;

	void AddFont(const std::string& name, Font* font)
	{
		m_fontSystem->m_fontMap.insert({ name, font });
		m_fontSystem->LoadFont(*font);
	}

	virtual void Clear(const glm::vec4& color) = 0;
	virtual void PreRender(bool font = false) = 0;
	virtual void PostRender(bool font = false) = 0;

	virtual std::unique_ptr<Framebuffer> CreateFramebuffer() = 0;

	inline void SetScreenSize(int width, int height)
	{
		m_screenSize = { width, height };
	}

	inline glm::vec2 GetScreenSize() const { return m_screenSize; }

	inline float GetScreenSizePercentage() { return m_screenSize.x / m_defaultScreenSize.x; }

	void SetCamera(const Camera& camera);
	void SetCamera(CameraComponent camera);

	inline void SetViewProjectionMatrix(const glm::mat4& viewProj)
	{
		m_viewProjectionMatrix = viewProj;
	}

	static glm::vec4 s_quadVertices[4];

protected:
	std::unique_ptr<FontSystem> m_fontSystem;
	std::unique_ptr<Shader> m_quadShader;
	std::shared_ptr<VertexBuffer> m_quadVertexBuffer;
	std::shared_ptr<VertexArray> m_quadVertexArray;

	std::vector<DrawTextCommand> m_drawTextCommands;

	uint32_t m_quadIndexCount = 0;
	QuadVertex* m_quadVertexBufferBase = nullptr;
	QuadVertex* m_quadVertexBufferPtr = nullptr;

	uint32_t m_textureIndex = 1;
	std::array<Texture*, MAX_TEXTURES> m_textures;

	Camera *m_camera;
	CameraComponent m_cameraComponent;
	glm::vec2 m_screenSize;
	glm::vec2 m_defaultScreenSize;
	glm::mat4 m_viewProjectionMatrix;

	friend class RenderSystem;  

private:
	void StartBatch();
	void NextBatch();
};
