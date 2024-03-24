#pragma once

#include <memory>
#include <functional>
#include "Asset/AssetManager.h"
#include "../Wrappers/ImGui_AssetWrapper.h"
#include "Asset/Asset.h"

class EditorAssetManager : public AssetManager
{
public:
	EditorAssetManager();


private:
	std::map<std::string, std::unique_ptr<Asset>> m_editorAssetMap;
};