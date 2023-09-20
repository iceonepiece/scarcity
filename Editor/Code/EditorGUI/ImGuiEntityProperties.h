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

	template <typename T>
	void RenderAddComponent(entt::registry& registry, entt::entity entity)
	{
		if (ImGui::Selectable(T::Name().c_str()))
		{
			if (registry.try_get<T>(entity) == nullptr)
				registry.emplace<T>(entity);
		}
	}

private:
	EditorLayer& m_editor;
	ImGuiAssetProperties m_imguiAssetProperties;
};