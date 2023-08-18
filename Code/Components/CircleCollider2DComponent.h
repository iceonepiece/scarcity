#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <box2d/b2_body.h>

struct CircleCollider2DComponent
{
	glm::vec2 offset = { 0.0f, 0.0f };
	float radius = 0.5;
};

static void RenderImGui(CircleCollider2DComponent& circle)
{
    ImGui::DragFloat2("Offset", glm::value_ptr(circle.offset));
    ImGui::InputFloat("Radius", &circle.radius);
}