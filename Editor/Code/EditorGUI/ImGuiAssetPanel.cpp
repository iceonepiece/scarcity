#include "imgui/imgui.h"
#include <IconsFontAwesome6.h>
#include "ImGuiAssetPanel.h"
#include <string>
#include "Platforms/OpenGL/OpenGLTexture.h"
#include "../EditorLayer.h"

ImGuiAssetPanel::ImGuiAssetPanel(EditorLayer& editor)
	: m_editor(editor)
	, m_baseDirectory(std::filesystem::current_path())
	, m_currentDirectory(m_baseDirectory)
{
}

void ImGuiAssetPanel::RenderUnsupportedFile(const std::filesystem::path& path)
{
	std::string useIcon = (ICON_FA_FILE " ");

	ImGuiTreeNodeFlags flags = (m_editor.GetSelectedPath() == path ? ImGuiTreeNodeFlags_Selected : 0);
	//flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	flags |= ImGuiTreeNodeFlags_Leaf;

	bool opened = ImGui::TreeNodeEx(path.c_str(), flags, std::string(useIcon + path.filename().string()).c_str());

	if (ImGui::IsItemClicked())
		m_editor.SetSelectedPath(path);

	if (opened)
	{
		ImGui::TreePop();
	}
}

void ImGuiAssetPanel::RenderNativeScript(NativeScriptAsset& nativeScriptAsset, ImGuiTreeNodeFlags flags, AssetEventFunction callback)
{
	std::string useIcon = (ICON_FA_FILE_CODE " ");

	flags |= ImGuiTreeNodeFlags_Leaf;

	bool opened = ImGui::TreeNodeEx(nativeScriptAsset.GetPath().string().c_str(), flags, (useIcon + nativeScriptAsset.GetName()).c_str());

	if (opened)
	{
		ImGui::TreePop();
	}

	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		ImGui::SetDragDropPayload("NATIVE_SCRIPT_FILE", &nativeScriptAsset, sizeof(nativeScriptAsset));

		ImGui::EndDragDropSource();
	}
}

void ImGuiAssetPanel::RenderPrefab(PrefabAsset& prefabAsset, ImGuiTreeNodeFlags flags, AssetEventFunction callback)
{
	std::string useIcon = ICON_FA_CUBE;

	flags |= ImGuiTreeNodeFlags_Leaf;

	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(140, 200, 255, 255));
	bool opened = ImGui::TreeNodeEx(prefabAsset.GetPath().string().c_str(), flags, useIcon.c_str());
	ImGui::PopStyleColor();

	callback();

	if (ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload("PREFAB_ASSET", &prefabAsset, sizeof(prefabAsset));

		ImGui::EndDragDropSource();
	}

	ImGui::SameLine();
	ImGui::Text(prefabAsset.GetPath().filename().string().c_str());

	if (ImGui::BeginPopupContextItem(prefabAsset.GetPath().string().c_str()))
	{
		if (ImGui::MenuItem("Delete"))
		{
			m_showDeleteModal = true;
			m_onActionAsset = &prefabAsset;
		}

		ImGui::EndPopup();
	}

	if (opened)
		ImGui::TreePop();
}

void ImGuiAssetPanel::RenderTexture(TextureAsset& textureAsset, ImGuiTreeNodeFlags flags, AssetEventFunction callback, OnSelectSpriteFunction selectSpriteFn, const std::string& note)
{
	std::string useIcon = (ICON_FA_IMAGE " ");

	flags |= ImGuiTreeNodeFlags_OpenOnArrow;

	if (note != "")
		flags &= ~ImGuiTreeNodeFlags_Selected;

	bool opened = ImGui::TreeNodeEx(textureAsset.GetPath().string().c_str(), flags, (useIcon + textureAsset.GetName()).c_str());

	callback();

	if (opened)
	{
		std::vector<SpriteAsset>& spriteAssets = textureAsset.GetSpriteAssets();

		for (auto& sprite : spriteAssets)
		{
			if (ImGui::Selectable(sprite.GetSprite().GetName().c_str(), sprite.GetSprite().GetName() == note, ImGuiSelectableFlags_DontClosePopups))
				selectSpriteFn(sprite);
		}

		ImGui::TreePop();
	}

	/*
	if (ImGui::BeginDragDropSource())
	{
		std::filesystem::path relativePath(path);
		const wchar_t* itemPath = relativePath.c_str();
		ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
		ImGui::EndDragDropSource();
	}
	*/
}

void ImGuiAssetPanel::RenderAudio(AudioAsset& audioAsset, ImGuiTreeNodeFlags flags, AssetEventFunction callback)
{
	std::string useIcon = (ICON_FA_MUSIC " ");

	flags |= ImGuiTreeNodeFlags_Leaf;

	bool opened = ImGui::TreeNodeEx(audioAsset.GetPath().string().c_str(), flags, (useIcon + audioAsset.GetName()).c_str());

	callback();

	if (opened)
	{
		ImGui::TreePop();
	}
}

void ImGuiAssetPanel::RenderAnimatorController(AnimatorControllerAsset& animControllerAsset, ImGuiTreeNodeFlags flags, AssetEventFunction callback)
{
	std::string useIcon = (ICON_FA_PERSON_RUNNING " ");

	flags |= ImGuiTreeNodeFlags_Leaf;

	bool opened = ImGui::TreeNodeEx(animControllerAsset.GetPath().string().c_str(), flags, (useIcon + animControllerAsset.GetName()).c_str());

	callback();

	if (opened)
	{
		ImGui::TreePop();
	}
}


void ImGuiAssetPanel::RenderFolder(const std::filesystem::path& path, ImGuiTreeNodeFlags flags, AssetEventFunction callback)
{
	std::string useIcon = ICON_FA_FOLDER;

	flags |= ImGuiTreeNodeFlags_Leaf;

	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(235, 231, 108, 255));
	bool opened = ImGui::TreeNodeEx(path.c_str(), flags, useIcon.c_str());
	ImGui::PopStyleColor();

	callback();

	ImGui::SameLine();
	ImGui::Text(path.filename().string().c_str());

	if (opened)
		ImGui::TreePop();
}

void ImGuiAssetPanel::Render()
{
	ImGui::Begin("Project");

	if (m_currentDirectory != std::filesystem::path(m_baseDirectory))
	{
		if (ImGui::Button("Back"))
			m_currentDirectory = m_currentDirectory.parent_path();
	}

	ImGui::SameLine();
	ImGui::Text(m_currentDirectory.string().c_str());
	ImGui::Separator();

	ImGui::BeginChild("Asset Area");
	static float padding = 18.0f;
	static float thumbnailSize = 512.0f;
	float cellSize = thumbnailSize + padding;

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);
	if (columnCount < 1)
		columnCount = 1;

	ImGui::Columns(columnCount, 0, false);

	for (auto& directoryEntry : std::filesystem::directory_iterator(m_currentDirectory))
	{
		const auto& path = directoryEntry.path();

		if (path.extension() == ".meta")
			continue;

		ImGuiTreeNodeFlags flags = (m_editor.GetSelectedPath() == path ? ImGuiTreeNodeFlags_Selected : 0);
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		std::string filenameString = path.filename().string();
		//ImGui::PushID(path.c_str());

		if (directoryEntry.is_directory())
		{
			RenderFolder(path, flags, [&]()
				{
					if (ImGui::IsItemClicked())
						m_editor.SetSelectedPath(path);

					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						m_currentDirectory /= path.filename();
				});

		}
		else if (Asset* asset = m_editor.GetAsset(path))
		{
			if (asset->GetType() == AssetType::Texture)
			{
				TextureAsset* textureAsset = static_cast<TextureAsset*>(asset);
				RenderTexture(*textureAsset, flags,
					[&]()
					{
						if (ImGui::IsItemClicked())
							m_editor.SetSelectedPath(textureAsset->GetPath());
					},

					[&](SpriteAsset& spriteAsset)
					{
						//m_editor.SetSelectedPath(textureAsset->GetPath(), sprite.GetName());
						m_editor.SetSelectedAsset(&spriteAsset, spriteAsset.GetSprite().GetName());
					},
					m_editor.GetSelectedObject().note
				);
			}
			else if (asset->GetType() == AssetType::Audio)
			{
				AudioAsset* audioAsset = static_cast<AudioAsset*>(asset);
				RenderAudio(*audioAsset, flags, [&]()
				{
					if (ImGui::IsItemClicked())
						m_editor.SetSelectedPath(audioAsset->GetPath());
				});
			}
			else if (asset->GetType() == AssetType::NativeScript)
			{
				NativeScriptAsset* nativeScriptAsset = static_cast<NativeScriptAsset*>(asset);
				RenderNativeScript(*nativeScriptAsset, flags, [&]()
				{
					if (ImGui::IsItemClicked())
						m_editor.SetSelectedPath(nativeScriptAsset->GetPath());
				});
			}
			else if (asset->GetType() == AssetType::Prefab)
			{
				PrefabAsset* prefabAsset = static_cast<PrefabAsset*>(asset);
				RenderPrefab(*prefabAsset, flags, [&]()
				{
					if (ImGui::IsItemClicked())
						m_editor.SetSelectedPath(prefabAsset->GetPath());
				});
			}
			else if (asset->GetType() == AssetType::AnimatorController)
			{
				AnimatorControllerAsset* animControllerAsset = static_cast<AnimatorControllerAsset*>(asset);
				RenderAnimatorController(*animControllerAsset, flags, [&]()
				{
					if (ImGui::IsItemClicked())
						m_editor.SetSelectedPath(animControllerAsset->GetPath());
				});
			}
			else if (asset->GetType() == AssetType::Scene)
			{
				flags |= ImGuiTreeNodeFlags_Leaf;

				std::string useIcon = (ICON_FA_WINDOW_MAXIMIZE " ");

				bool opened = ImGui::TreeNodeEx(path.string().c_str(), flags, (useIcon + path.filename().string()).c_str());

				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
					m_editor.OpenScene(path);

				if (opened)
					ImGui::TreePop();
			}
		}
		else
		{
			flags |= ImGuiTreeNodeFlags_Leaf;

			std::string useIcon = (ICON_FA_FILE_LINES " ");

			bool opened = ImGui::TreeNodeEx(path.string().c_str(), flags, (useIcon + path.filename().string()).c_str());

			if (ImGui::IsItemClicked())
				m_editor.SetSelectedPath(path);

			if (opened)
				ImGui::TreePop();
		}

		//ImGui::TextWrapped(filenameString.c_str());

		//ImGui::PopID();
		ImGui::NextColumn();
	}



	ImGui::Columns(1);
	ImGui::EndChild();

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY"))
		{
			//entt::entity payload_entity = *(entt::entity*)payload->Data;
			Entity payloadEntity = *(Entity*)payload->Data;
			std::cout << "Drag Drop: " << (int)payloadEntity.GetEntity() << std::endl;
			m_editor.CreatePrefab(payloadEntity.GetEntity(), m_currentDirectory);
		}
		ImGui::EndDragDropTarget();
	}

	if (m_showDeleteModal && m_onActionAsset)
		ImGui::OpenPopup("Delete selected asset?");

	if (ImGui::BeginPopupModal("Delete selected asset?", NULL, NULL))
	{
		ImGui::Text(m_onActionAsset->GetPath().string().c_str());
		ImGui::Text("You cannot undo the delete assets action.");

		if (ImGui::Button("Delete"))
		{
			FileSystem::RemoveFile(m_onActionAsset->GetPath());
			m_showDeleteModal = false;
			m_onActionAsset = nullptr;
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::Button("Cancel"))
		{
			m_showDeleteModal = false;
			m_onActionAsset = nullptr;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	//ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
	//ImGui::SliderFloat("Padding", &padding, 0, 32);

	// TODO: status bar
	ImGui::End();

}