#pragma once

#include "File/FileSystem.h"

class EditorLayer;

enum class SpriteMode
{
	Single,
	Multiple
};

struct SpriteResource
{
	SpriteMode mode = SpriteMode::Single;
};

class ImGuiEntityProperties
{
public:
	ImGuiEntityProperties(EditorLayer& editor);
	void Render();
	void RenderResource(const Resource& resource);
	void ApplySpriteData();
	void RevertSpriteData();

private:
	EditorLayer& m_editor;
	SpriteResource m_currentSpriteResource;
};