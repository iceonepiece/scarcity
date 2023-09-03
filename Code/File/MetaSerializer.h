#pragma once

#include <filesystem>
#include <nlohmann/json.hpp>
#include "Asset/TextureAsset.h"

using json = nlohmann::json;

class MetaSerializer
{
public:
	static bool SerializeImage(TextureAsset& sprite, const std::filesystem::path& path)
	{
		std::cout << "SerializeImage: " << path << std::endl;
		std::ofstream metaFile;

		metaFile.open(path.string() + ".meta");

		if (metaFile.is_open())
		{
			json imageJson;
			imageJson["spriteMode"] = (int)sprite.m_spriteMode;

			json json_sprites = json::array();

			if (sprite.m_spriteMode == SpriteMode::Multiple)
			{
				float spriteWidth = sprite.m_texture->GetWidth() / sprite.m_cols;
				float spriteHeight = sprite.m_texture->GetHeight() / sprite.m_rows;

				int count = 0;
				for (int i = 0; i < sprite.m_rows; i++)
				{
					for (int j = 0; j < sprite.m_cols; j++)
					{
						json json_sprite;
						json_sprite["name"] = sprite.m_path.stem().string() + "_" + std::to_string(count++);
						json_sprite["x"] = spriteWidth * j;
						json_sprite["y"] = spriteHeight * i;
						json_sprite["width"] = spriteWidth;
						json_sprite["height"] = spriteHeight;
						json_sprites.push_back(json_sprite);
					}
				}
			}

			imageJson["sprites"] = json_sprites;

			metaFile <<  imageJson.dump(4);
		}
		else
		{
			std::cerr << "Error opening the file!" << std::endl;
			return false;
		}

		metaFile.close();

		return true;
	}

	static bool DeserializeImage(TextureAsset& sprite, const std::filesystem::path& path)
	{
		std::ifstream metaFile;

		metaFile.open(path.string() + ".meta");

		if (metaFile.is_open())
		{
			json imageJson = json::parse(metaFile);
			sprite.m_spriteMode = (SpriteMode)imageJson["spriteMode"];
		}
		else
		{
			std::cerr << "Error opening the file!" << std::endl;
			return false;
		}

		metaFile.close();

		return true;
	}
};