#pragma once

#include <filesystem>
#include <IconsFontAwesome6.h>
#include "File/FileSystem.h"
#include "ImGui_Window.h"
#include "Animations/AnimatorController.h"

class ImGuiSelectAnimatorControllerWindow : public ImGui_Window
{
public:
	ImGuiSelectAnimatorControllerWindow(EditorLayer& editor, const std::filesystem::path& path);
	virtual void Render() override;

	inline std::filesystem::path GetSelectedPath() { return m_currentDirectory; }

	inline AnimatorController* GetSelectedController() { return m_selectedController; }

private:
	AnimatorController* m_selectedController;
	std::filesystem::path m_baseDirectory;
	std::filesystem::path m_currentDirectory;
};