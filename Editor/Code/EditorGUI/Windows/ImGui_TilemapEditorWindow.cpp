#include "ImGui_TilemapEditorWindow.h"
#include "Graphics/Texture.h"
#include "Asset/AssetManager.h"

ImGui_TilemapEditorWindow::ImGui_TilemapEditorWindow(EditorLayer& editor)
	: ImGui_Window(editor, "Tilemap Editor")
{
	s_instance = this;
}

void ImGui_TilemapEditorWindow::Render()
{
	static const int SPRITE_WIDTH = 48;
	static const int HALF_SPRITE_WIDTH = SPRITE_WIDTH / 2;

	if (!m_isOpen)
		return;

	if (ImGui::Begin("Tilemap Editor", &m_isOpen, ImGuiWindowFlags_NoScrollbar))
	{
        if (m_tilemap != nullptr && m_tilemap->image != nullptr)
        {
			if (Texture* texture = m_tilemap->image->GetTexture())
			{
				ImGui::Text("size = %d x %d", texture->GetWidth(), texture->GetHeight());

				int mousePosX = static_cast<int>(ImGui::GetMousePos().x - ImGui::GetCursorScreenPos().x);
				int mousePosY = static_cast<int>(ImGui::GetMousePos().y - ImGui::GetCursorScreenPos().y);

				std::cout << mousePosX << " : " << mousePosY << std::endl;

				ImGui::Image((ImTextureID)texture->GetRendererID(), ImVec2((float)texture->GetWidth(), (float)texture->GetHeight()), { 0, 1 }, { 1, 0 });
			}
	
        }
	}

    ImGui::End();
}