#pragma once

#include <glm/glm.hpp>
#include "../Graphics/Texture.h"

class Sprite
{
public:
	Sprite() = default;

	Sprite(Texture* texture, glm::vec2 size, glm::vec2 grid)
		: m_texture(texture)
		, m_ratio(glm::normalize(size))
	{
		m_left = grid.x * size.x / m_texture->GetWidth();
		m_right = m_left + size.x / m_texture->GetWidth();
		m_bottom = grid.y * size.y / m_texture->GetHeight();
		m_top = m_bottom + size.y / m_texture->GetHeight();
	}


	void SetTexture(Texture* texture)
	{
		m_texture = texture;
	}

	Texture* GetTexture()
	{
		return m_texture;
	}

	float GetLeft()
	{
		return m_left;
	}

	float GetRight()
	{
		return m_right;
	}

	float GetBottom()
	{
		return m_bottom;
	}

	float GetTop()
	{
		return m_top;
	}

	glm::vec2 GetRatio()
	{
		return m_ratio;
	}

private:
	Texture* m_texture;
	glm::vec2 m_ratio;
	float m_left;
	float m_right;
	float m_bottom;
	float m_top;
};