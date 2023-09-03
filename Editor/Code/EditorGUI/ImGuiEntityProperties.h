#pragma once

#include "File/FileSystem.h"
#include "ImGuiAssetProperties.h"

class EditorLayer;

class ImGuiEntityProperties
{
public:
	ImGuiEntityProperties(EditorLayer& editor);
	void Render();

private:
	EditorLayer& m_editor;
	ImGuiAssetProperties m_imguiAssetProperties;
};