#pragma once

class Texture
{
public:
	virtual void Bind(uint32_t slot = 0) = 0;
	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;
	virtual uint64_t GetRendererID() const = 0;
	virtual bool operator==(const Texture& other) const = 0;
};

class NullTexture : public Texture
{
	virtual void Bind(uint32_t slot) override {}
	virtual int GetWidth() const override { return 0; }
	virtual int GetHeight() const override { return 0; }
	virtual uint64_t GetRendererID() const override { return 0; }
	virtual bool operator==(const Texture& other) const { return false; }
};

static NullTexture g_nullTexture;