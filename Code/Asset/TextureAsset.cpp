#include "TextureAsset.h"
#include "Core/ResourceAPI.h"
#include "File/MetaSerializer.h"

TextureAsset::TextureAsset(const std::filesystem::path& path, Texture* texture)
	: Asset(path, AssetType::Texture)
	, m_texture(texture)
{}

TextureAsset::TextureAsset(const std::filesystem::path& path)
	: Asset(path, AssetType::Texture)
{
	m_texture = ResourceAPI::LoadTexture(path.string(), path.string().c_str());
	MetaSerializer::DeserializeImage(*this, path);
}

