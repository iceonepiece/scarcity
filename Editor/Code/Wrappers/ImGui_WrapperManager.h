#pragma once

#include <unordered_map>
#include "ImGui_AssetWrapper.h"
#include "Asset/Asset.h"

class ImGui_WrapperManager
{
public:
	static ImGui_AssetWrapper* GetWrapper(Asset& asset);

private:
	static std::unique_ptr<ImGui_AssetWrapper> s_assetWrappers[(size_t)AssetType::Count];
};