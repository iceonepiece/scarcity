#pragma once

#include <filesystem>
#include <IconsFontAwesome6.h>
#include "File/FileSystem.h"
#include "ImGuiWindow.h"
#include "FSM/FiniteStateMachine.h"
#include "Animations/AnimatorController.h"

class ImGuiSelectAnimatorControllerWindow : public ImGuiWindow
{
public:
	ImGuiSelectAnimatorControllerWindow(EditorLayer& editor, const std::filesystem::path& path);
	virtual void Render() override;

	inline std::filesystem::path GetSelectedPath() { return m_currentDirectory; }

	inline std::string GetSelectedControllerName() { return m_selectedControllerName; }

private:
	std::string m_selectedControllerName;
	std::filesystem::path m_baseDirectory;
	std::filesystem::path m_currentDirectory;
};