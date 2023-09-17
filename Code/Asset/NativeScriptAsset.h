#pragma once

#include "Asset.h"

class NativeScriptAsset : public Asset
{
public:
	NativeScriptAsset(const std::filesystem::path& path)
		: Asset(path, AssetType::NativeScript)
	{}
};