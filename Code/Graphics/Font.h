#pragma once

#include <map>
#include "Asset/Asset.h"

struct Character {
	unsigned int TextureID; // ID handle of the glyph texture
	glm::ivec2   Size;      // Size of glyph
	glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
	unsigned int Advance;   // Horizontal offset to advance to next glyph
};

typedef std::map<char, Character> GlyphMap;

class Font : public Asset
{
public:

	inline static unsigned int DEFAULT_FONT_SIZES[] = {
		16, 18, 20, 24, 28, 32, 48, 64
	};

	Font(const std::filesystem::path& path)
		: Asset(path, AssetType::Font)
	{
	}

	bool HasSize(unsigned int size)
	{
		return m_glyphs.find(size) != m_glyphs.end();
	}
	
	std::map<char, Character>& GetCharacters(unsigned int size)
	{
		return m_glyphs[size];
	}

	void AddGlypMap(unsigned int size, const GlyphMap& glyphMap)
	{
		m_glyphs.insert({ size, glyphMap });
	}

protected:
	std::map<unsigned int, GlyphMap> m_glyphs;
};