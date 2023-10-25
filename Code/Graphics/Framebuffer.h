#pragma once

class Framebuffer
{
public:
	virtual ~Framebuffer() = default;

	virtual void Bind() = 0;
	virtual void Unbind() = 0;
	virtual void Rescale(float width, float height) = 0;
	virtual unsigned int GetID() = 0;
};