#include "TextureAsset.h"
#include "File/MetaSerializer.h"

TextureAsset::TextureAsset(const std::filesystem::path& path, Texture* texture)
	: Asset(path, AssetType::Texture)
	, m_texture(texture)
{}

TextureAsset::TextureAsset(const std::filesystem::path& path)
	: Asset(path, AssetType::Texture)
{
	//m_texture = ResourceAPI::LoadTexture(path.string(), path.string().c_str());
	m_texture = Application::Get().GetAssetManager().LoadTexture(path.string(), path.string().c_str());
	MetaSerializer::DeserializeImage(*this, path);
}

