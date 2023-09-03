#pragma once

#include "Asset.h"

#pragma once

#include "File/FileSystem.h"
#include "Graphics/Texture.h"
#include "Animations/Sprite.h"

enum class SpriteMode
{
	Single,
	Multiple
};

class TextureAsset : public Asset
{
public:
	TextureAsset(const std::filesystem::path& path, Texture* texture)
		: Asset(path, AssetType::Texture)
		, m_texture(texture)
	{}

	virtual ~TextureAsset() = default;
	virtual std::string GetTypeString() override { return "Texture"; }

	inline Texture* GetTexture() { return m_texture; }

	inline SpriteMode GetSpriteMode() { return m_spriteMode; }
	inline void SetSpriteMode(SpriteMode mode) { m_spriteMode = mode; }

	inline int& GetRows() { return m_rows; }
	inline int& GetCols() { return m_cols; }

	friend class MetaSerializer;

private:
	SpriteMode m_spriteMode = SpriteMode::Single;
	Texture* m_texture = nullptr;
	int m_cols = 0;
	int m_rows = 0;

	std::vector<Sprite> m_sprites;
};