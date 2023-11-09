#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class EdgeOnCell
{
	Left,
	Bottom,
	Right,
	Top
};

struct GridEdge
{
	int cellX;
	int cellY;
	EdgeOnCell onCell;
};

using GridPolygon = std::vector<GridEdge>;


struct GridComponent
{
	static std::string Name() { return "Grid"; }

	float size = 1.0f;
	glm::vec4 color { 1.0f };

	std::map<std::pair<int, int>, int> cellMap;
	std::vector<GridPolygon> polygons;
};

static void DoSerialize(const GridComponent& grid, json& entityJson)
{
	entityJson[GridComponent::Name()]["size"] = grid.size;

	json colorJson = json::array();
	colorJson.push_back(grid.color.r);
	colorJson.push_back(grid.color.g);
	colorJson.push_back(grid.color.b);
	colorJson.push_back(grid.color.a);
	entityJson[GridComponent::Name()]["color"] = colorJson;

	json cellsJson = json::array();

	for (auto& [key, value] : grid.cellMap)
	{
		cellsJson.push_back({ key.first,  key.second });
	}
	entityJson[GridComponent::Name()]["cells"] = cellsJson;
}

static void DoDeserialize(GridComponent& grid, json& gridJson)
{
	grid.size = gridJson["size"].get<float>();

	auto& colorJson = gridJson["color"];
	glm::vec4 color{ colorJson[0].get<float>(), colorJson[1].get<float>(), colorJson[2].get<float>(), colorJson[3].get<float>() };
	grid.color = color;

	if (gridJson["cells"].is_array())
	{
		for (auto& cellJson : gridJson["cells"])
		{
			grid.cellMap.insert({ std::make_pair(cellJson[0], cellJson[1]), 1 });
		}
	}
}