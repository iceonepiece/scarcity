#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

class Texture
{
public:
	Texture();
	~Texture();

	void Generate(const char* filename, bool alpha = false);
	void Bind();

	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }

private:
	unsigned int m_id;
	int m_width;
	int m_height;
};