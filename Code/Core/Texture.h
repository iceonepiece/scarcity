#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

class Texture
{
public:
	Texture();
	~Texture();

	void Generate(const char* filename, bool alpha = false);
	void Bind();

private:
	unsigned int m_id;
};