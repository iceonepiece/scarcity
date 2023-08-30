#pragma once

class Texture
{
public:
	virtual void Bind() = 0;
	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;
	virtual uint32_t GetRendererID() const = 0;
};