#include "ImGui_Components.h"
#include "Core/Application.h"
#include <imgui/imgui.h>

void RenderImGui(LuaScriptComponent&  luaScript)
{
    ImGui::Text("File Name: ");
    ImGui::SameLine();

    std::string scriptFilename = "";

    if (luaScript.script)
    {
        scriptFilename = luaScript.script->GetName();
    }

    ImGui::Button(scriptFilename.c_str(), ImVec2(200, 0));

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("LUA_SCRIPT_FILE"))
        {
            LuaScript* script = (LuaScript*)(payload->Data);
            luaScript.script = (LuaScript*)Project::GetActive()->GetAssetManager().GetAsset(script->GetPath());
        }

        ImGui::EndDragDropTarget();
    }
}