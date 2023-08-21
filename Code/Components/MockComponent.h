#pragma once

#include <string>

struct MockComponent
{
	static std::string Name() { return "Mock"; }
	int number;
};

static void RenderImGui(MockComponent& mock)
{
	ImGui::InputInt("Number", &mock.number);
}