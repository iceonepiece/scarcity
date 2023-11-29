#include "Image.h"
#include "File/MetaSerializer.h"

Image::Image(const std::filesystem::path& path, Texture* texture)
	: Asset(path, AssetType::Image)
	, m_texture(texture)
{}

Image::Image(const std::filesystem::path& path)
	: Asset(path, AssetType::Image)
{
	//m_texture = ResourceAPI::LoadTexture(path.string(), path.string().c_str());
	m_texture = Application::Get().GetAssetManager().LoadTexture(path.string(), path.string().c_str());
	MetaSerializer::DeserializeImage(*this, path);
}

