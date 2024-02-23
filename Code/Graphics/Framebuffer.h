#pragma once

class Framebuffer
{
public:
	virtual ~Framebuffer() = default;

	virtual void Bind() = 0;
	virtual void Unbind() = 0;
	virtual void Rescale(unsigned int width, unsigned int height) = 0;
	virtual unsigned int GetID() = 0;

	unsigned int GetWidth() const { return m_width; }
	unsigned int GetHeight() const { return m_height; }

protected:
	unsigned int m_width = 1;
	unsigned int m_height = 1;
};