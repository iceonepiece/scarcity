#pragma once

#include "Asset.h"
#include "File/FileSystem.h"
#include "Graphics/Texture.h"
#include "Animations/Sprite.h"

enum class SpriteMode
{
	Single,
	Multiple
};

class TextureAsset;

class SpriteAsset : public Asset
{
public:
	SpriteAsset(Sprite sprite, TextureAsset& textureAsset)
		: Asset("")
		, m_sprite(sprite)
		, m_textureAsset(textureAsset)
	{

	}

	inline TextureAsset& GetTextureAsset() { return m_textureAsset; }
	inline Sprite& GetSprite() { return m_sprite; }

private:
	Sprite m_sprite;
	TextureAsset& m_textureAsset;
};

class TextureAsset : public Asset
{
public:
	TextureAsset(const std::filesystem::path& path);
	TextureAsset(const std::filesystem::path& path, Texture* texture);

	virtual ~TextureAsset() = default;
	virtual std::string GetTypeString() override { return "Texture"; }

	inline Texture* GetTexture() { return m_texture; }

	inline SpriteMode GetSpriteMode() { return m_spriteMode; }
	inline void SetSpriteMode(SpriteMode mode) { m_spriteMode = mode; }

	inline int& GetRows() { return m_rows; }
	inline int& GetCols() { return m_cols; }

	inline std::vector<SpriteAsset>& GetSpriteAssets() { return m_spriteAssets; }

	friend class MetaSerializer;

private:
	SpriteMode m_spriteMode = SpriteMode::Single;
	Texture* m_texture = nullptr;
	int m_cols = 0;
	int m_rows = 0;

	std::vector<SpriteAsset> m_spriteAssets;
};