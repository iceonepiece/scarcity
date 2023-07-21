#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Parser
{
public:
	static void LoadJSON(const std::string& fileName)
	{
		std::ifstream f(fileName);
		json data = json::parse(f);

		const std::string& xx = data.at("name");
		auto& kk = data.at("entities");

		std::cout << xx << std::endl;

		for (auto& i : kk)
		{
			std::cout << i.at("id") << " : " << i.at("tag") << std::endl;
		}
	}
};