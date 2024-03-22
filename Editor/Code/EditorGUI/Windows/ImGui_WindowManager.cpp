#include "ImGui_WindowManager.h"
#include "ImGuiSelectSpriteWindow.h"
#include "ImGuiSelectAnimatorControllerWindow.h"
#include "ImGuiTagEditorWindow.h"
#include "ImGui_AnimationClipWindow.h"
#include "ImGui_ProjectSettingsWindow.h"
#include "ImGui_LuaEditorWindow.h"
#include "ImGui_TilemapEditorWindow.h"
#include "ImGui_AnimatorWindow.h"
#include "Project/Project.h"
#include "../../EditorLayer.h"

ImGui_Window* ImGui_WindowManager::GetWindow(ImGuiWindowType::Type type)
{
    return s_imGuiWindows[type].get();
}

void ImGui_WindowManager::Initialize(EditorLayer& editor)
{
    s_imGuiWindows[ImGuiWindowType::SelectSprite] = std::make_unique<ImGuiSelectSpriteWindow>(editor, editor.GetProject()->GetDirectory());
    s_imGuiWindows[ImGuiWindowType::SelectAnimatorController] = std::make_unique<ImGuiSelectAnimatorControllerWindow>(editor, editor.GetProject()->GetDirectory());
    s_imGuiWindows[ImGuiWindowType::Tags] = std::make_unique<ImGuiTagEditorWindow>(editor);
    s_imGuiWindows[ImGuiWindowType::AnimationClip] = std::make_unique<ImGui_AnimationClipWindow>(editor);
    s_imGuiWindows[ImGuiWindowType::Animator] = std::make_unique<ImGui_AnimatorWindow>(editor);
    s_imGuiWindows[ImGuiWindowType::ProjectSettings] = std::make_unique<ImGui_ProjectSettingsWindow>(editor);
    s_imGuiWindows[ImGuiWindowType::LuaEditor] = std::make_unique<ImGui_LuaEditorWindow>(editor);
    s_imGuiWindows[ImGuiWindowType::TilemapEditor] = std::make_unique<ImGui_TilemapEditorWindow>(editor);
}

void ImGui_WindowManager::Render()
{
    s_imGuiWindows[ImGuiWindowType::ProjectSettings]->Render();
    s_imGuiWindows[ImGuiWindowType::AnimationClip]->Render();
    s_imGuiWindows[ImGuiWindowType::LuaEditor]->Render();
    s_imGuiWindows[ImGuiWindowType::Animator]->Render();
    s_imGuiWindows[ImGuiWindowType::TilemapEditor]->Render();
}
