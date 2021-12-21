#include "Shader.h"

Shader::Shader()
{
}

void Shader::Compile(const char* vertexSource, const char* fragmentSource)
{
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);

  m_id = glCreateProgram();
  glAttachShader(m_id, vertexShader);
  glAttachShader(m_id, fragmentShader);
  glLinkProgram(m_id);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
}

void Shader::SetInt(const char* name, int value)
{
  glUniform1i(glGetUniformLocation(m_id, name), value);
}

void Shader::SetVector3f(const char* name, glm::vec3& value)
{
  glUniform3f(glGetUniformLocation(m_id, name), value.x, value.y, value.z);
}

void Shader::SetVector4f(const char* name, glm::vec4& value)
{
  glUniform4f(glGetUniformLocation(m_id, name), value.x, value.y, value.z, value.w);
}


void Shader::SetMatrix4(const char* name, glm::mat4& value)
{
  glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, &value[0][0]);
}

void Shader::Use()
{
  glUseProgram(m_id);
}
