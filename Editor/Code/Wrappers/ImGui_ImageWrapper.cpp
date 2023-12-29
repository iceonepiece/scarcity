#include "ImGui_ImageWrapper.h"
#include <IconsFontAwesome6.h>
#include "../EditorLayer.h"


void ImGui_ImageWrapper::RenderInspector()
{
    std::string name = m_image->GetName() + " (Texture 2D)";
    ImGui::Text(name.c_str());

    const char* spriteModeStrings[] = { "Single", "Multiple" };
    const char* currentSpriteModeString = spriteModeStrings[(int)m_image->GetSpriteMode()];
    if (ImGui::BeginCombo("Sprite Mode", currentSpriteModeString))
    {
        for (int i = 0; i < 2; i++)
        {
            bool isSelected = currentSpriteModeString == spriteModeStrings[i];
            if (ImGui::Selectable(spriteModeStrings[i], isSelected))
            {
                currentSpriteModeString = spriteModeStrings[i];
                m_image->SetSpriteMode((SpriteMode)i);
            }

            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }

    if (m_image->GetSpriteMode() == SpriteMode::Multiple)
    {
        ImGui::InputInt("Rows", &m_image->GetRows());
        ImGui::InputInt("Cols", &m_image->GetCols());
    }

    if (ImGui::Button("Revert"))
    {
        //RevertSpriteData();
    }

    ImGui::SameLine();

    if (ImGui::Button("Apply"))
    {
        if (MetaSerializer::SerializeImage(*m_image, m_image->GetPath()))
            MetaSerializer::DeserializeImage(*m_image, m_image->GetPath());
    }

    std::string pathString = m_image->GetPath().string();

    Texture* texture = nullptr;

    AssetManager& assetManager = Project::GetActive()->GetAssetManager();

    if (assetManager.HasTexture(pathString))
    {
        texture = &assetManager.GetTexture(pathString);
    }
    else
    {
        std::cout << "Load when clicked: " << pathString << std::endl;
        texture = assetManager.LoadTexture(pathString, pathString.c_str());
    }

    if (texture != nullptr)
    {
        ImGui::Text("pointer = %p", texture->GetRendererID());
        ImGui::Text("size = %d x %d", texture->GetWidth(), texture->GetHeight());
        ImGui::Image((ImTextureID)texture->GetRendererID(), ImVec2((float)texture->GetWidth(), (float)texture->GetHeight()), { 0, 1 }, { 1, 0 });
    }
}

void ImGui_ImageWrapper::RenderBrowser(EditorLayer& editor)
{
    std::string useIcon = (ICON_FA_IMAGE " ");

    ImGuiTreeNodeFlags flags = (editor.GetSelectedPath() == m_image->GetPath() ? ImGuiTreeNodeFlags_Selected : 0);
    flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
    flags |= ImGuiTreeNodeFlags_OpenOnArrow;

    if (editor.GetSelectedObject().note != "")
        flags &= ~ImGuiTreeNodeFlags_Selected;

    bool opened = ImGui::TreeNodeEx(m_image->GetPath().string().c_str(), flags, (useIcon + m_image->GetName()).c_str());

    if (ImGui::IsItemClicked())
        editor.SetSelectedObject(EditorObjectType::Asset, m_image);

    if (opened)
    {
        std::vector<Sprite>& sprites = m_image->GetSprites();

        for (int i = 0; i < sprites.size(); i++)
        {
            Sprite& sprite = sprites[i];
            if (ImGui::Selectable(sprite.GetName().c_str(), sprite.GetName() == editor.GetSelectedObject().note, ImGuiSelectableFlags_DontClosePopups))
            {
                EditorObject& editorObject = editor.SetSelectedObject(EditorObjectType::Asset, &sprite);
                editorObject.note = sprite.GetName();
            }
        }

        ImGui::TreePop();
    }
}