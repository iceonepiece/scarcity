#include "Sprite.h"
#include "Graphics/Image.h"

Sprite::Sprite()
	: Asset("", AssetType::Sprite)
{

}

Sprite::Sprite(Image* image, glm::vec2 size, glm::vec2 grid)
	: Asset("", AssetType::Sprite)
	, m_image(image)
	, m_ratio(glm::normalize(size))
{
	if (Texture* texture = image->GetTexture())
	{
		m_left = grid.x * size.x / texture->GetWidth();
		m_right = m_left + size.x / texture->GetWidth();
		m_bottom = grid.y * size.y / texture->GetHeight();
		m_top = m_bottom + size.y / texture->GetHeight();
	}
}

Sprite::Sprite(const std::string& name, Image* image, float x, float y, float width, float height)
	: Asset("", AssetType::Sprite)
	, m_name(name)
	, m_image(image)
	, m_x(x)
	, m_y(y)
	, m_width(width)
	, m_height(height)
	, m_ratio(glm::normalize(glm::vec2{ width, height }))
{
	if (Texture* texture = image->GetTexture())
	{
		m_left = m_x / texture->GetWidth();
		m_right = m_left + m_width / texture->GetWidth();
		m_bottom = m_y / texture->GetHeight();
		m_top = m_bottom + m_height / texture->GetHeight();
	}
}

Texture* Sprite::GetTexture()
{
	return m_image->GetTexture();
}