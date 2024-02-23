#pragma once

#include <imgui/imgui.h>
#include "Asset/Asset.h"
#include <IconsFontAwesome6.h>

class EditorLayer;

class ImGui_AssetWrapper
{
public:
	virtual ~ImGui_AssetWrapper() = default;

	virtual void RenderBrowser(EditorLayer& editor) {}
	virtual void RenderInspector() {}
	virtual void SetAsset(Asset* asset) {}
};