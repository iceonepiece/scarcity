#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Graphics/Texture.h"

class OpenGLTexture : public Texture
{
public:
	OpenGLTexture();
	OpenGLTexture(const std::string& path);
	~OpenGLTexture();

	bool Generate(const char* filename, bool alpha = false);
	virtual void Bind(uint32_t slot = 0) override;

	virtual int GetWidth() const override { return m_width; }
	virtual int GetHeight() const override { return m_height; }
	virtual uint64_t GetRendererID() const override { return m_id; }
	
	virtual bool operator==(const Texture& other) const override
	{
		return m_id == other.GetRendererID();
	}

private:
	GLuint m_id;
	int m_width;
	int m_height;
};