#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct Cell
{
	int x;
	int y;
};

struct GridComponent
{
	static std::string Name() { return "Grid"; }
	float size;
	std::vector<Cell> cells;
};

static void DoSerialize(const GridComponent& base, json& entityJson)
{
}

static void DoDeserialize(GridComponent& base, json& baseJson)
{
}