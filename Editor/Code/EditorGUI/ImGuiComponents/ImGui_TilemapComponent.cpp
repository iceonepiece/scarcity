#include "ImGui_Components.h"
#include "../Windows/ImGui_TilemapEditorWindow.h"
#include "../../EditorLayer.h"

void RenderImGui(TilemapComponent& tilemap)
{
    std::string imageFilename = tilemap.originalImage != nullptr ? tilemap.originalImage->GetName() : "";

    if (ImGui::Button(imageFilename.c_str(), ImVec2(200, 0)))
    {
        ImGui_TilemapEditorWindow* tilemapEditor = ImGui_TilemapEditorWindow::Get();
        tilemapEditor->SetTilemap(&tilemap);
        tilemapEditor->SetOpen(true);
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("IMAGE_FILE"))
        {
            if (Image* image = (Image*)payload->Data)
            {
                tilemap.originalImage = (Image*)Project::GetActive()->GetAsset(image->GetPath());
            }
        }

        ImGui::EndDragDropTarget();
    }
}