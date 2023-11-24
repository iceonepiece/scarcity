#pragma once

class AnimatorState;
class AnimatorTransition;

class ImGui_AnimatorInspector
{
public:
	void Render(AnimatorState& node);

private:
	AnimatorTransition* m_selectedTransition = nullptr;
};