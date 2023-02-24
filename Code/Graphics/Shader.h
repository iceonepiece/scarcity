#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Shader
{
public:
	Shader();
	~Shader();

	void Compile(const char* vertexSource, const char* fragmentSource);
	void Use();

	void SetInt(const char* name, int value);
	void SetFloat(const char* name, float value);
	void SetVector3f(const char* name, const glm::vec3& value);
	void SetVector4f(const char* name, const glm::vec4& value);
	void SetMatrix4(const char* name, const glm::mat4& value);

	unsigned int m_id;
};
