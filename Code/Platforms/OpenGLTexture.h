#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include "../Graphics/Texture.h"

class OpenGLTexture : public Texture
{
public:
	OpenGLTexture();
	~OpenGLTexture();

	void Generate(const char* filename, bool alpha = false);
	void Bind();

	virtual int GetWidth() const override { return m_width; }
	virtual int GetHeight() const override { return m_height; }

private:
	GLuint m_id;
	int m_width;
	int m_height;
};