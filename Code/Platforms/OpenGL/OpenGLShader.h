#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Graphics/Shader.h"

class OpenGLShader : public Shader
{
public:
	OpenGLShader();
	~OpenGLShader();

	virtual void Compile(const std::string& vertexSource, const std::string& fragmentSource) override;
	virtual void Use() override;

	virtual void SetInt(const std::string& name, int value) override;
	virtual void SetFloat(const std::string& name, float value) override;
	virtual void SetVector3f(const std::string& name, const glm::vec3& value) override;
	virtual void SetVector4f(const std::string& name, const glm::vec4& value) override;
	virtual void SetMatrix4(const std::string& name, const glm::mat4& value) override;

	unsigned int m_id;
};
