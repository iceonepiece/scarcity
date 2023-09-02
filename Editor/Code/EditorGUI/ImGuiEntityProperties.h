#pragma once

#include "File/FileSystem.h"
#include "Graphics/Sprite.h"
#include "ImGuiResourceProperties.h"

class EditorLayer;

class ImGuiEntityProperties
{
public:
	ImGuiEntityProperties(EditorLayer& editor);
	void Render();

private:
	EditorLayer& m_editor;
	ImGuiResourceProperties m_imguiResourceProperties;
};