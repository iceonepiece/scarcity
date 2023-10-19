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
		if (RenderAddComponent<T>(registry, entity))
		{
			if (CanvasComponent* canvas = registry.try_get<CanvasComponent>(entity))
			{
				AdjustCanvas(registry.get<T>(entity), canvas);
				return true;
			}
		}

		return false;
	}

private:
	EditorLayer& m_editor;
	ImGuiAssetProperties m_imguiAssetProperties;
};