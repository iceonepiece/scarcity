#pragma once

#include <filesystem>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class MetaSerializer
{
public:
	static bool SerializeImage(const std::filesystem::path& path)
	{
		std::ofstream metaFile;

		metaFile.open(path.string() + ".meta");

		if (metaFile.is_open())
		{
			json imageJson;
			imageJson["spriteMode"] = 0;
			imageJson["sprites"] = json::array();

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
};