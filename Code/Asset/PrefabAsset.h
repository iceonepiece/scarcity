#pragma once

#include "Asset.h"

class PrefabAsset : public Asset
{
public:
	PrefabAsset(const std::filesystem::path& path)
		: Asset(path, AssetType::Prefab)
	{}

	virtual std::string GetTypeString() override { return "Prefab"; }
};