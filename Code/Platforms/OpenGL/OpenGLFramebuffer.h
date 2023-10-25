#pragma once

#include "Graphics/Framebuffer.h"
#include <glad/glad.h>

class OpenGLFramebuffer : public Framebuffer
{
public:
	OpenGLFramebuffer();
	virtual ~OpenGLFramebuffer();

	virtual void Bind() override;
	virtual void Unbind() override;
	virtual void Rescale(float width, float height) override;

	inline virtual unsigned int GetID() { return m_ID; }

private:
	GLuint m_ID;
	GLuint m_FBO;
	GLuint m_RBO;

	unsigned int m_width;
	unsigned int m_height;
};