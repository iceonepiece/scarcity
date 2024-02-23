#pragma once

#include "ImGui_AssetWrapper.h"
#include "Lua/LuaScript.h"

class ImGui_LuaScriptWrapper : public ImGui_AssetWrapper
{
public:
	virtual void RenderBrowser(EditorLayer& editor) override;

	virtual void RenderInspector()
	{
		std::string name = m_luaScript->GetPath().stem().string() + " (Lua Script)";

		ImGui::Text(name.c_str());
	}

	virtual void SetAsset(Asset* asset)
	{
		m_luaScript = static_cast<LuaScript*>(asset);
	}

private:
	LuaScript* m_luaScript;
};