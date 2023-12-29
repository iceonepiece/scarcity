#pragma once

#include "ImGui_AssetWrapper.h"
#include "Entity/Prefab.h"
#include "../EditorGUI/ImGuiComponents/ImGuiComponentRenderer.h"

class ImGui_PrefabWrapper : public ImGui_AssetWrapper
{
public:
    virtual void RenderBrowser(EditorLayer& editor) override;

	virtual void RenderInspector()
	{
        std::string name = m_prefab->GetPath().stem().string() + " (Prefab)";
        ImGui::Text(name.c_str());

        Entity& entity = m_prefab->GetEntity();
        auto& registry = entity.GetRegistry();

        ImGui::PushID((int)entity.GetEntity());

        std::apply([&](auto... componentTypes) {
            (ImGuiComponentRenderer::RenderComponent<decltype(componentTypes)>(registry, entity.GetEntity()), ...);
            }, ComponentList{});

        ImGui::PopID();
	}

    virtual void SetAsset(Asset* asset)
    {
		m_prefab = static_cast<Prefab*>(asset);
    }

private:
	Prefab* m_prefab;
};