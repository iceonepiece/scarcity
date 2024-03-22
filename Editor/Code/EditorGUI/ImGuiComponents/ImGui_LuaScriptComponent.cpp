#include "ImGui_Components.h"
#include "Core/Application.h"
#include <imgui/imgui.h>
#include "../../EditorLayer.h"
#include "../Windows/ImGui_WindowManager.h"
#include "../Windows/ImGui_LuaEditorWindow.h"

void RenderImGui(LuaScriptComponent&  luaScript)
{
    ImGui::Text("File Name: ");
    ImGui::SameLine();

    std::string scriptFilename = "";

    if (luaScript.script)
    {
        scriptFilename = luaScript.script->GetName();
    }

    if (ImGui::Button(scriptFilename.c_str(), ImVec2(200, 0)))
    {
        ImGui_LuaEditorWindow* luaEditor = ImGui_LuaEditorWindow::Get();
        luaEditor->LoadScript(luaScript.script->GetPath().string());
        luaEditor->SetOpen(true);
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("LUA_SCRIPT_FILE"))
        {
			if (LuaScript* script = (LuaScript*)payload->Data)
			{
                luaScript.script = (LuaScript*)Project::GetActive()->GetAsset(script->GetPath());
			}
        }

        ImGui::EndDragDropTarget();
    }
}