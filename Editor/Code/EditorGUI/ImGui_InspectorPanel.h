#pragma once

#include <entt/entt.hpp>
#include <imgui/imgui.h>
#include "File/FileSystem.h"
#include "Components/Components.h"

class EditorLayer;

class ImGui_InspectorPanel
{
public:
	ImGui_InspectorPanel(EditorLayer& editor);
	void Render();

	template <typename T>
	bool RenderAddComponent(entt::registry& registry, entt::entity entity)
	{
		if (ImGui::Selectable(T::Name().c_str()))
		{
			if (registry.try_get<T>(entity) == nullptr)
			{
				registry.emplace<T>(entity);
				return true;
			}
		}

		return false;
	}

	template <typename T>
	bool RenderAddUIComponent(entt::registry& registry, entt::entity entity)
	{
		return false;
	}

private:
	EditorLayer& m_editor;
};