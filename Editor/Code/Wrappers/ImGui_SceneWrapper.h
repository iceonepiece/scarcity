#pragma once

#include "ImGui_AssetWrapper.h"
#include "Scene/Scene.h"

class ImGui_SceneWrapper : public ImGui_AssetWrapper
{
public:
	virtual ~ImGui_SceneWrapper() = default;

	virtual void RenderBrowser(EditorLayer& editor) override;

	virtual void SetAsset(Asset* asset) override
	{
		m_scene = dynamic_cast<Scene*>(asset);
	}

private:
	Scene* m_scene;
};