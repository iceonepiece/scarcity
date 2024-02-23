#pragma once

#include "Animations/AnimatorController.h"
#include "ImGui_AssetWrapper.h"

class ImGui_AnimatorControllerWrapper : public ImGui_AssetWrapper
{
public:
	virtual void RenderBrowser(EditorLayer& editor) override;

	virtual void RenderInspector()
	{
		std::string name = m_animController->GetPath().stem().string() + " (Animator Controller)";

		ImGui::Text(name.c_str());
	}

	virtual void SetAsset(Asset* asset)
	{
		m_animController = dynamic_cast<AnimatorController*>(asset);
	}

private:
	AnimatorController* m_animController;
};