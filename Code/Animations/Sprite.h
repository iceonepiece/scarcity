#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <string>
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

	Sprite(const std::string& name, Texture* texture, float x, float y, float width, float height)
		: m_name(name)
		, m_texture(texture)
		, m_x(x)
		, m_y(y)
		, m_width(width)
		, m_height(height)
		, m_ratio(glm::normalize(glm::vec2{ width, height }))
	{
		m_left = m_x / m_texture->GetWidth();
		m_right = m_left + m_width / m_texture->GetWidth();
		m_bottom = m_y / m_texture->GetHeight();
		m_top = m_bottom + m_height / m_texture->GetHeight();

		std::cout << "Left " << m_left << " : RIght " << m_right << std::endl;
		std::cout << "Bottom " << m_bottom << " : Top " << m_top << std::endl;
	}

	inline std::string GetName() { return m_name; }

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
	std::string m_name;
	Texture* m_texture;
	glm::vec2 m_ratio;
	float m_left;
	float m_right;
	float m_bottom;
	float m_top;

	float m_x;
	float m_y;
	float m_width;
	float m_height;
};