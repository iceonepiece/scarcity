#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <string>
#include "Graphics/Texture.h"
#include "Asset/Asset.h"

class Image;

class Sprite : public Asset
{
public:
	Sprite();
	Sprite(Image* texture, glm::vec2 size, glm::vec2 grid);
	Sprite(const std::string& name, Image* texture, float x, float y, float width, float height);

	inline std::string GetName() { return m_name; }

	//inline void SetTexture(Texture* texture) { m_texture = texture; }
	//inline Texture* GetTexture() { return m_textureAsset->Get; }

	inline float GetLeft() { return m_left; }
	inline float GetRight() { return m_right; }
	inline float GetBottom() { return m_bottom; }
	inline float GetTop() { return m_top; }

	inline float GetWidth() { return m_width; }
	inline float GetHeight() { return m_height; }

	inline glm::vec2 GetRatio() { return m_ratio; }
	inline void SetRatio(const glm::vec2& ratio) { m_ratio = ratio; }
	inline Image* GetImage() { return m_image; }
	Texture* GetTexture();

private:
	Image* m_image;

	std::string m_name;
	//Texture* m_texture;

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