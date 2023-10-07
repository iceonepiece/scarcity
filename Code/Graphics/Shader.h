#pragma once

#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
	virtual ~Shader() = default;
	virtual void Compile(const std::string& vertexSource, const std::string& fragmentSource) = 0;
	virtual void Use() = 0;

	virtual void SetInt(const std::string& name, int value) = 0;
	virtual void SetFloat(const std::string& name, float value) = 0;
	virtual void SetVector3f(const std::string& name, const glm::vec3& value) = 0;
	virtual void SetVector4f(const std::string& name, const glm::vec4& value) = 0;
	virtual void SetMatrix4(const std::string& name, const glm::mat4& value) = 0;
};
