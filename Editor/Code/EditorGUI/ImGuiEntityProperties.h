#pragma once

#include <entt/entt.hpp>
#include <imgui/imgui.h>
#include "File/FileSystem.h"
#include "ImGuiAssetProperties.h"
#include "Components/Components.h"

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