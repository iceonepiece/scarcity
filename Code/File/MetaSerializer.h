#pragma once

#include <vector>
#include <filesystem>
#include <nlohmann/json.hpp>
#include "Animations/Sprite.h"
#include "Asset/TextureAsset.h"
#include "Core/ResourceAPI.h"

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
				int spriteWidth = sprite.m_texture->GetWidth() / sprite.m_cols;
				int spriteHeight = sprite.m_texture->GetHeight() / sprite.m_rows;

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

	static bool DeserializeImage(TextureAsset& textureAsset, const std::filesystem::path& path)
	{
		std::cout << "DeserializeImage\n" << std::endl;
		std::ifstream metaFile;

		metaFile.open(path.string() + ".meta");

		if (metaFile.is_open())
		{
			ResourceManager* resourceManager = ResourceAPI::GetResourceManager();
			resourceManager->RemoveSpriteAssets(textureAsset.m_spriteAssets);
			textureAsset.m_spriteAssets.clear();

			json imageJson = json::parse(metaFile);
			textureAsset.m_spriteMode = (SpriteMode)imageJson["spriteMode"];

			if (textureAsset.m_spriteMode == SpriteMode::Multiple)
			{
				json json_sprites = imageJson["sprites"];

				for (auto& json_sprite : json_sprites)
				{
					SpriteAsset spriteAsset {
						Sprite {
							json_sprite["name"],
							textureAsset.m_texture,
							json_sprite["x"],
							json_sprite["y"],
							json_sprite["width"],
							json_sprite["height"]
						},
						textureAsset
					};

					textureAsset.m_spriteAssets.push_back(spriteAsset);
				}
			}
			else if (textureAsset.m_spriteMode == SpriteMode::Single)
			{
				SpriteAsset spriteAsset {
					Sprite {
						textureAsset.m_path.stem().string(),
						textureAsset.m_texture,
						0,
						0,
						(float)textureAsset.GetTexture()->GetWidth(),
						(float)textureAsset.GetTexture()->GetHeight()
					},
					textureAsset
				};

				textureAsset.m_spriteAssets.push_back(spriteAsset);
			}

			resourceManager->AddSpriteAssets(textureAsset.m_spriteAssets);
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