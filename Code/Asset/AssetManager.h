#pragma once

#include <memory>
#include <unordered_map>
#include <filesystem>
#include "Asset.h"

class AssetManager
{
public:
	Asset* GetAsset(const std::filesystem::path& path);
	Asset* LoadAsset(const std::filesystem::path& path);
	bool AssetExists(const std::filesystem::path& path);
private:
	std::unordered_map<std::string, std::unique_ptr<Asset>> m_assetMap;
};