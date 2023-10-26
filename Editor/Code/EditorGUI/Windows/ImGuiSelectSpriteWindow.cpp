#include "ImGuiSelectSpriteWindow.h"
#include "../ImGuiAssetPanel.h"
#include "../../EditorLayer.h"


ImGuiSelectSpriteWindow::ImGuiSelectSpriteWindow(EditorLayer& editor, const std::filesystem::path& path)
	: ImGuiWindow_(editor)
	, m_baseDirectory(path)
	, m_currentDirectory(path)
{

}

void ImGuiSelectSpriteWindow::Render()
{
	if (m_currentDirectory != std::filesystem::path(m_baseDirectory))
	{
		if (ImGui::Button("Back"))
			m_currentDirectory = m_currentDirectory.parent_path();

		ImGui::Separator();
	}

	for (auto& directoryEntry : std::filesystem::directory_iterator(m_currentDirectory))
	{
		const auto& path = directoryEntry.path();

		if (directoryEntry.is_directory())
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf;
			ImGuiAssetPanel::RenderFolder(path, flags, [&]()
			{
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					m_currentDirectory /= path.filename();
			});
		}
		else if (FileSystem::IsImageFile(path))
		{
			if (TextureAsset* asset = dynamic_cast<TextureAsset*>(EditorLayer::GetAsset(path)))
			{
				ImGuiAssetPanel::RenderTexture(*asset, 0, [&](){}, [&](SpriteAsset& sprite)
				{
					m_selectedSprite = &sprite.GetSprite();
				});
			}
		}
	}
}