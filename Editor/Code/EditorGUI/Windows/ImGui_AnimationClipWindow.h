#pragma once

#include <set>
#include "ImGui_Window.h"
#include "Animations/AnimationClip.h"

class ImGui_AnimationClipWindow : public ImGui_Window
{
public:
	ImGui_AnimationClipWindow(EditorLayer& editor);
	virtual void Render() override;
	void SetAnimationClip(AnimationClip* animationClip);

private:
	void Close();
	void ResetPreview();

	Image* m_selectedImage = nullptr;
	AnimationClip* m_animationClip = nullptr;
	size_t m_selectedImageIndex = 0;
	int m_selectedSpriteIndex = -1;
	std::set<size_t> m_selectedSprites;
	Texture* m_currentTexture = nullptr;
	bool m_isPlaying = false;
	int m_currentFrame = 1;
	float m_timer = 0.0f;
	float m_frameTime = 0.1f;
};