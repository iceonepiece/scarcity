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

    Image* image = nullptr;
	int order;

	std::map<std::pair<int, int>, unsigned int> data;
};

static void DoSerialize(const TilemapComponent& tilemap, json& entityJson)
{
	json tilemapJson = json::object();
	tilemapJson["imageID"] = 0;

	if (tilemap.image != nullptr)
	{
		tilemapJson["imageID"] = (uint64_t)tilemap.image->GetID();
	}

	json dataJson = json::array();

	for (auto& [key, value] : tilemap.data)
	{
		json block = json::object();
		block["row"] = key.first;
		block["col"] = key.second;
		block["index"] = value;
		dataJson.push_back(block);
	}
	tilemapJson["data"] = dataJson;
	
	entityJson[TilemapComponent::Name()] = tilemapJson;
}

static void DoDeserialize(TilemapComponent& tilemap, json& tilemapJson)
{
	UniqueID imageID = tilemapJson["imageID"].get<uint64_t>();
	if (Image* image = dynamic_cast<Image*>(Project::GetActive()->GetAssetManager().GetAssetByID(imageID)))
	{
		tilemap.image = image;
	}

	if (tilemapJson["data"].is_array())
	{
		for (auto& blockJson : tilemapJson["data"])
		{
			tilemap.data.insert({ 
				std::make_pair(blockJson["row"], blockJson["col"]),
				blockJson["index"]
			});
		}
	}
}