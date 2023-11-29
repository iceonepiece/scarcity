#pragma once

#include <imgui/imgui.h>
#include "Animations/AnimatorState.h"

class ImGui_AnimatorState : public AnimatorState
{
private:
	ImVec2 m_position;
	bool m_selected;
};