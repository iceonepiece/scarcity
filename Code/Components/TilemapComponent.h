#pragma once

#include "Graphics/Image.h"
#include "Project/Project.h"

#include <string>
#include <map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct TilemapComponent
{
	static std::string Name() { return "Tilemap"; }

	Image* originalImage = nullptr;
	std::vector<Sprite> tilemapSprites;

	int rows = 1;
	int cols = 1;

	int order = 0;

	std::map<std::pair<int, int>, std::pair<int, int>> data;

	void GenerateSprites()
	{
		if (originalImage == nullptr)
			return;

		if (Texture* texture = originalImage->GetTexture())
		{
			int spriteWidth = texture->GetWidth() / cols;
			int spriteHeight = texture->GetHeight() / rows;

			for (int i = 0; i < rows; i++)
			{
				for (int j = 0; j < cols; j++)
				{
					Sprite sprite{
						std::to_string(i),
						originalImage,
						(float)spriteWidth * j,
						(float)spriteHeight * i,
						(float)spriteWidth,
						(float)spriteHeight
					};

					sprite.SetRatio({ 1.0f, 1.0f });
					tilemapSprites.push_back(sprite);
				}
			}
		}
	}
};

static void DoSerialize(const TilemapComponent& tilemap, json& entityJson)
{
	json tilemapJson = json::object();
	tilemapJson["imageID"] = 0;
	tilemapJson["order"] = tilemap.order;

	if (tilemap.originalImage != nullptr)
	{
		tilemapJson["imageID"] = (uint64_t)tilemap.originalImage->GetID();
		tilemapJson["rows"] = tilemap.rows;
		tilemapJson["cols"] = tilemap.cols;
	}

	json dataJson = json::array();

	for (auto& [key, value] : tilemap.data)
	{
		json tileJson = json::object();
		tileJson["worldPosition"] = key;
		tileJson["texturePosition"] = value;
		dataJson.push_back(tileJson);
	}
	tilemapJson["data"] = dataJson;
	
	entityJson[TilemapComponent::Name()] = tilemapJson;
}

static void DoDeserialize(TilemapComponent& tilemap, json& tilemapJson)
{
	UniqueID imageID = tilemapJson["imageID"].get<uint64_t>();
	if (Image* image = dynamic_cast<Image*>(Project::GetActive()->GetAssetManager().GetAssetByID(imageID)))
	{
		tilemap.originalImage = image;
		tilemap.rows = tilemapJson["rows"].get<unsigned int>();
		tilemap.cols = tilemapJson["cols"].get<unsigned int>();
		tilemap.GenerateSprites();
	}

	tilemap.order = tilemapJson["order"].get<int>();

	if (tilemapJson["data"].is_array())
	{
		for (auto& blockJson : tilemapJson["data"])
		{
			std::pair<int, int> worldPair = blockJson["worldPosition"].get<std::pair<int, int>>();
			std::pair<int, int> texturePair = blockJson["texturePosition"].get<std::pair<int, int>>();

			tilemap.data.insert({ worldPair, texturePair });
		}
	}
}