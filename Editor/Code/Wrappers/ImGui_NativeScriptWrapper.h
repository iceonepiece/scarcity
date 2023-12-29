#pragma once

#include "ImGui_AssetWrapper.h"
#include "NativeScript/NativeScript.h"

class ImGui_NativeScriptWrapper : public ImGui_AssetWrapper
{
public:
	virtual void RenderBrowser(EditorLayer& editor) override;
	virtual void RenderInspector() override;

	virtual void SetAsset(Asset* asset)
	{
		m_nativeScript = static_cast<NativeScript*>(asset);
	}

private:
	NativeScript* m_nativeScript;
};