#include "imgui/imgui.h"
#include <IconsFontAwesome6.h>
#include "ImGuiAssetPanel.h"
#include <string>
#include "Platforms/OpenGL/OpenGLTexture.h"
#include "../EditorLayer.h"
#include "Windows/ImGui_AnimationClipWindow.h"
#include "Animations/AnimationClip.h"

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

	//if (ImGui::IsItemClicked())
		//m_editor.SetSelectedPath(path);

	if (opened)
	{
		ImGui::TreePop();
	}
}

void ImGuiAssetPanel::RenderNativeScript(NativeScript& nativeScript, ImGuiTreeNodeFlags flags, AssetEventFunction callback)
{
	std::string useIcon = (ICON_FA_FILE_CODE " ");

	flags |= ImGuiTreeNodeFlags_Leaf;

	bool opened = ImGui::TreeNodeEx(nativeScript.GetPath().string().c_str(), flags, (useIcon + nativeScript.GetName()).c_str());

	if (opened)
	{
		ImGui::TreePop();
	}

	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		ImGui::SetDragDropPayload("NATIVE_SCRIPT_FILE", &nativeScript, sizeof(nativeScript));

		ImGui::EndDragDropSource();
	}
}

void ImGuiAssetPanel::RightClickMenu(Asset& asset)
{
	if (ImGui::BeginPopupContextItem(asset.GetPath().filename().string().c_str()))
	{
		if (ImGui::MenuItem("Delete"))
		{
			m_showDeleteModal = true;
			m_onActionAsset = &asset;
		}

		ImGui::EndPopup();
	}
}

void ImGuiAssetPanel::RenderPrefab(Prefab& prefab, ImGuiTreeNodeFlags flags, AssetEventFunction callback)
{
	std::string useIcon = ICON_FA_CUBE;

	flags |= ImGuiTreeNodeFlags_Leaf;

	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(140, 200, 255, 255));
	bool opened = ImGui::TreeNodeEx(prefab.GetPath().string().c_str(), flags, useIcon.c_str());
	ImGui::PopStyleColor();

	callback();

	if (ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload("PREFAB_ASSET", &prefab, sizeof(prefab));

		ImGui::EndDragDropSource();
	}

	ImGui::SameLine();
	ImGui::Text(prefab.GetPath().filename().string().c_str());

	RightClickMenu(prefab);

	if (opened)
		ImGui::TreePop();
}

void ImGuiAssetPanel::RenderImage(Image& image, ImGuiTreeNodeFlags flags, AssetEventFunction callback, OnSelectSpriteFunction selectSpriteFn, const std::string& note)
{
	std::string useIcon = (ICON_FA_IMAGE " ");

	flags |= ImGuiTreeNodeFlags_OpenOnArrow;

	if (note != "")
		flags &= ~ImGuiTreeNodeFlags_Selected;

	bool opened = ImGui::TreeNodeEx(image.GetPath().string().c_str(), flags, (useIcon + image.GetName()).c_str());

	callback();

	if (opened)
	{
		//std::vector<SpriteAsset>& spriteAssets = image.GetSpriteAssets();
		std::vector<Sprite>& sprites = image.GetSprites();

		for (int i = 0; i < sprites.size(); i++)
		{
			Sprite& sprite = sprites[i];
			if (ImGui::Selectable(sprite.GetName().c_str(), sprite.GetName() == note, ImGuiSelectableFlags_DontClosePopups))
				selectSpriteFn(sprite, i);
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

void ImGuiAssetPanel::RenderAudioClip(AudioClip& audioClip, ImGuiTreeNodeFlags flags, AssetEventFunction callback)
{
	std::string useIcon = (ICON_FA_MUSIC " ");

	flags |= ImGuiTreeNodeFlags_Leaf;

	bool opened = ImGui::TreeNodeEx(audioClip.GetPath().string().c_str(), flags, (useIcon + audioClip.GetName()).c_str());

	callback();

	if (opened)
	{
		ImGui::TreePop();
	}
}

void ImGuiAssetPanel::RenderAnimatorController(AnimatorController& animControllerAsset, ImGuiTreeNodeFlags flags, AssetEventFunction callback)
{
	std::string useIcon = (ICON_FA_CIRCLE_NODES " ");

	flags |= ImGuiTreeNodeFlags_Leaf;

	bool opened = ImGui::TreeNodeEx(animControllerAsset.GetPath().string().c_str(), flags, (useIcon + animControllerAsset.GetName()).c_str());

	callback();

	RightClickMenu(animControllerAsset);

	if (opened)
	{
		ImGui::TreePop();
	}
}

void ImGuiAssetPanel::RenderAnimationClip(AnimationClip& animClip, ImGuiTreeNodeFlags flags, AssetEventFunction callback)
{
	std::string useIcon = ICON_FA_PERSON_RUNNING;

	flags |= ImGuiTreeNodeFlags_Leaf;

	bool opened = ImGui::TreeNodeEx(animClip.GetPath().string().c_str(), flags, (useIcon + animClip.GetName()).c_str());

	callback();

	RightClickMenu(animClip);

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

void ImGuiAssetPanel::RenderAddAssetButton()
{
	if (ImGui::Button(ICON_FA_PLUS ICON_FA_CARET_DOWN))
	{
		ImGui::OpenPopup("Add Asset");
	}

	if (ImGui::BeginPopup("Add Asset"))
	{
		ImGui::Text("Add Asset");
		ImGui::Separator();

		if (ImGui::Selectable("Animator Controller"))
			m_addingAssetType = AssetType::AnimatorController;

		if (ImGui::Selectable("Animation Clip"))
			m_addingAssetType = AssetType::AnimationClip;

		ImGui::EndPopup();
	}

	if (m_addingAssetType != AssetType::None)
	{
		ImGui::OpenPopup("Add Asset Name");
	}

	if (ImGui::BeginPopupModal("Add Asset Name", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Add Asset Name");
		ImGui::Separator();

		ImGui::InputText("Name", &m_addingAssetName);

		if (ImGui::Button("Add"))
		{
			if (m_addingAssetType == AssetType::AnimatorController)
			{
				AnimatorController animController;
				AnimationSerializer::Serialize(animController, m_currentDirectory / (m_addingAssetName + ".controller"));
			}
			else if (m_addingAssetType == AssetType::AnimationClip)
			{
				AnimationClip animClip(m_addingAssetName);
				AnimationSerializer::Serialize(animClip, m_currentDirectory / (m_addingAssetName + ".anim"));
			}

			m_addingAssetType = AssetType::None;
			m_addingAssetName = "";
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			m_addingAssetType = AssetType::None;
			m_addingAssetName = "";
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void ImGuiAssetPanel::Render()
{
	ImGui::Begin("Project");

	RenderAddAssetButton();

	ImGui::SameLine();

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
			if (asset->GetType() == AssetType::Image)
			{
				Image* image = static_cast<Image*>(asset);
				RenderImage(*image, flags,
					[&]()
					{
						if (ImGui::IsItemClicked())
							m_editor.SetSelectedObject(EditorObjectType::Asset, image);
					},

					//[&](SpriteAsset& spriteAsset)
					[&](Sprite& sprite, size_t index)
					{
						EditorObject& editorObject = m_editor.SetSelectedObject(EditorObjectType::Asset, &sprite);
						editorObject.note = sprite.GetName();
					},
					m_editor.GetSelectedObject().note
				);
			}
			else if (asset->GetType() == AssetType::Audio)
			{
				AudioClip* audioClip = static_cast<AudioClip*>(asset);
				RenderAudioClip(*audioClip, flags, [&]()
				{
					if (ImGui::IsItemClicked())
						m_editor.SetSelectedObject(EditorObjectType::Asset, audioClip);
				});
			}
			else if (asset->GetType() == AssetType::NativeScript)
			{
				NativeScript* nativeScript = static_cast<NativeScript*>(asset);
				RenderNativeScript(*nativeScript, flags, [&]()
				{
					if (ImGui::IsItemClicked())
						m_editor.SetSelectedObject(EditorObjectType::Asset, nativeScript);
				});
			}
			else if (asset->GetType() == AssetType::Prefab)
			{
				Prefab* prefab = static_cast<Prefab*>(asset);
				RenderPrefab(*prefab, flags, [&]()
				{
					if (ImGui::IsItemClicked())
						m_editor.SetSelectedObject(EditorObjectType::Asset, prefab);
				});
			}
			else if (asset->GetType() == AssetType::AnimatorController)
			{
				AnimatorController* animControllerAsset = static_cast<AnimatorController*>(asset);
				RenderAnimatorController(*animControllerAsset, flags, [&]()
				{
					if (ImGui::IsItemClicked())
					{
						m_editor.SetSelectedObject(EditorObjectType::Asset, animControllerAsset);
						m_editor.SetAnimatorController(*animControllerAsset);
					}
				});
			}
			else if (asset->GetType() == AssetType::AnimationClip)
			{
				AnimationClip* animClipAsset = static_cast<AnimationClip*>(asset);
				RenderAnimationClip(*animClipAsset, flags, [&]()
				{
					if (ImGui::IsItemClicked())
						m_editor.SetSelectedObject(EditorObjectType::Asset, animClipAsset);

					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						ImGui_AnimationClipWindow* animClipWindow = static_cast<ImGui_AnimationClipWindow*>(m_editor.GetImGuiWindow(ImGuiWindowType::AnimationClip));
						animClipWindow->SetAnimationClip(animClipAsset);
						animClipWindow->SetOpen(true);
					}
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

			//if (ImGui::IsItemClicked())
				//m_editor.SetSelectedPath(path);

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

		ImGui::SameLine();

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