#pragma once

#include <imgui/imgui.h>
#include <string>
#include "Components/NativeScriptComponent.h"

static void RenderImGui(NativeScriptComponent& script)
{
	ImGui::Text("File Name: ");
	ImGui::SameLine();
	ImGui::Button(script.className.c_str(), ImVec2(200, 0));

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("NATIVE_SCRIPT_FILE"))
        {
            NativeScriptAsset* nativeScriptAsset = (NativeScriptAsset*)payload->Data;
            std::cout << "Drop native script name: " << nativeScriptAsset->GetPath() << std::endl;
            script.className = nativeScriptAsset->GetPath().stem().string();
        }

        ImGui::EndDragDropTarget();
    }
}