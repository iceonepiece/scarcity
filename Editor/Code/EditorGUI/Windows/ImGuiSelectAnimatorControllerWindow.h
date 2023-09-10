#pragma once

#include <filesystem>
#include <IconsFontAwesome6.h>
#include "File/FileSystem.h"
#include "ImGuiWindow.h"
#include "FSM/FiniteStateMachine.h"

class ImGuiSelectAnimatorControllerWindow : public ImGuiWindow
{
public:
	ImGuiSelectAnimatorControllerWindow(EditorLayer& editor, const std::filesystem::path& path);
	virtual void Render() override;

	inline std::filesystem::path GetSelectedPath() { return m_currentDirectory; }

private:
	FiniteStateMachine* m_fsm;
	std::filesystem::path m_baseDirectory;
	std::filesystem::path m_currentDirectory;
};