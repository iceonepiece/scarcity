#include "ImGui_AnimationClipWindow.h"
#include "Core/Application.h"
#include <IconsFontAwesome6.h>
#include "../ImGuiUtils.h"

ImGui_AnimationClipWindow::ImGui_AnimationClipWindow(EditorLayer& editor)
	: ImGui_Window(editor, "Animation Clip")
{}

void ImGui_AnimationClipWindow::SetAnimationClip(AnimationClip* animationClip)
{
	if (animationClip == nullptr)
		return;

	m_animationClip = animationClip;
	m_selectedImage = animationClip->GetImage();

	if (m_selectedImage != nullptr)
		m_currentTexture = m_selectedImage->GetTexture();

	m_selectedSprites.clear();

	if (Image* image = m_animationClip->GetImage())
	{
		for (auto& index : m_animationClip->GetSpriteIndices())
		{
			m_selectedSprites.insert(index);
		}
	}

}

void ImGui_AnimationClipWindow::Render()
{
	static const int SPRITE_WIDTH = 48;
	static const int HALF_SPRITE_WIDTH = SPRITE_WIDTH / 2;

	if (m_animationClip == nullptr || !m_isOpen)
		return;

	if (ImGui::Begin(m_windowName.c_str(), &m_isOpen, ImGuiWindowFlags_NoScrollbar))
	{
		if (!m_isOpen)
		{
			this->Close();
			ImGui::End();
			return;
		}

		ImGui::Text(m_animationClip->GetName().c_str());

		auto& images = Application::Get().GetAssetManager().GetImages();
		std::vector<std::string> imageNames{ "None" };

		for (int i = 0; i < images.size(); i++)
		{
			imageNames.push_back(images[i]->GetName());

			if (images[i] == m_selectedImage)
				m_selectedImageIndex = i + 1;
		}

		unsigned int previousImageIndex = m_selectedImageIndex;

		ImGui::Text("Image: ");
		ImGui::SameLine();
		ImGuiUtils::RenderDropdownList("###Images", imageNames, m_selectedImageIndex);

		if (previousImageIndex != m_selectedImageIndex)
		{
			m_selectedImage = images[m_selectedImageIndex - 1];
			m_selectedSpriteIndex = -1;

			if (m_selectedImage != nullptr)
				m_currentTexture = m_selectedImage->GetTexture();

			m_selectedSprites.clear();
			ResetPreview();
		}

		if (ImGui::Button("Save"))
		{
			m_animationClip->SetImage(m_selectedImage);

			auto& spriteIndices = m_animationClip->GetSpriteIndices();
			spriteIndices.clear();

			for (auto& index : m_selectedSprites)
			{
				spriteIndices.push_back(index);
			}

			AnimationSerializer::Serialize(*m_animationClip, m_animationClip->GetPath());

			this->Close();
		}

		ImGui::Separator();

		if (m_selectedImage != nullptr)
		{
			int renderWidth = SPRITE_WIDTH;

			ImGui::BeginChild("Sprites", ImVec2(300, 0), true);
			
			auto& sprites = m_selectedImage->GetSprites();

			for (int i = 0; i < sprites.size(); i++)
			{
				Sprite& sprite = sprites[i];

				if (sprite.GetWidth() < HALF_SPRITE_WIDTH)
					renderWidth = HALF_SPRITE_WIDTH;
				else if (sprite.GetWidth() > SPRITE_WIDTH)
					renderWidth = SPRITE_WIDTH;

				ImVec2 renderSize{ (float)renderWidth, renderWidth * (sprite.GetRatio().y / sprite.GetRatio().x) };
				ImGui::Image((ImTextureID)m_currentTexture->GetRendererID(), renderSize, { sprite.GetLeft(), sprite.GetTop() }, { sprite.GetRight(), sprite.GetBottom() });

				ImGui::SameLine();

				ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2 {0.5f, 0.5f});

				if (ImGui::Selectable(sprite.GetName().c_str(), m_selectedSprites.contains(i), ImGuiSelectableFlags_DontClosePopups, ImVec2(0, 50)))
				{
					if (!ImGui::GetIO().KeyCtrl)
						m_selectedSprites.clear();

					if (m_selectedSprites.contains(i))
						m_selectedSprites.erase(i);
					else
						m_selectedSprites.insert(i);

					m_selectedSpriteIndex = i;
				}

				ImGui::PopStyleVar();
			}

			ImGui::EndChild();

			ImGui::SameLine();

			ImGui::BeginChild("Preview", ImVec2(0, 0));

			ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2{ 0.5f, 0.5f });
			
			ImGui::Text("Preview");
			ImGui::Text("Total Frames: %d", m_selectedSprites.size());

			ImGui::Text("Frame Time (Seconds): ");
			ImGui::SameLine();
			ImGui::InputFloat("##frameTime", &m_frameTime, 0);

			if (m_selectedSprites.size() > 0)
			{
				auto it = m_selectedSprites.begin();

				for (int i = 0; i < m_selectedSprites.size(); i++)
				{
					if (i == m_currentFrame - 1)
						break;
					it++;
				}

				if (it == m_selectedSprites.end())
				{
					m_currentFrame = 1;
					it = m_selectedSprites.begin();
				}

				auto& sprite = sprites[*it];

				if (m_currentTexture != nullptr)
				{
					const int SPRITE_WIDTH = 256;
					const int HALF_SPRITE_WIDTH = SPRITE_WIDTH / 2;

					int renderWidth = SPRITE_WIDTH;
					
					if (sprite.GetWidth() < HALF_SPRITE_WIDTH)
						renderWidth = HALF_SPRITE_WIDTH;
					else if (sprite.GetWidth() > SPRITE_WIDTH)
						renderWidth = SPRITE_WIDTH;

					ImVec2 renderSize{ (float)renderWidth, renderWidth * (sprite.GetRatio().y / sprite.GetRatio().x) };
					ImGui::Image((ImTextureID)m_currentTexture->GetRendererID(), renderSize, { sprite.GetLeft(), sprite.GetTop() }, { sprite.GetRight(), sprite.GetBottom() });
				}
			}

			if (m_isPlaying)
			{
				m_timer += ImGui::GetIO().DeltaTime;

				if (m_timer >= m_frameTime)
				{
					m_timer = 0.0f;
					m_currentFrame++;

					if (m_currentFrame > m_selectedSprites.size())
						m_currentFrame = 1;
				}

				if (ImGui::Button(ICON_FA_PAUSE))
					m_isPlaying = false;
			}
			else
			{
				if (ImGui::Button(ICON_FA_PLAY))
				{
					m_isPlaying = true;
					m_timer = 0.0f;
				}
			}


			ImGui::SameLine();

			if (ImGui::Button(ICON_FA_STOP))
			{
				m_isPlaying = false;
				m_currentFrame = 1;
				m_timer = 0.0f;
			}

			ImGui::SameLine();
			ImGui::SliderInt("Frame", &m_currentFrame, 1, m_selectedSprites.size());

			ImGui::PopStyleVar();

			ImGui::EndChild();
		}
		else
		{
			m_selectedSpriteIndex = -1;
		}
	}

	ImGui::End();
}

void ImGui_AnimationClipWindow::ResetPreview()
{
	m_isPlaying = false;
	m_timer = 0.0f;
	m_frameTime = 0.1f;
	m_currentFrame = 1;
}

void ImGui_AnimationClipWindow::Close()
{
	ResetPreview();
	m_isOpen = false;
}