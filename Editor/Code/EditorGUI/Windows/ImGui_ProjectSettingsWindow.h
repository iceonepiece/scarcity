#pragma once

#include <set>
#include "ImGui_Window.h"

class ImGui_ProjectSettingsWindow : public ImGui_Window
{
public:
	ImGui_ProjectSettingsWindow(EditorLayer& editor);
	virtual void Render() override;
};