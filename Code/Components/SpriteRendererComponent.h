#pragma once

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <imgui/imgui.h>

using json = nlohmann::json;

enum SpriteShape
{
	Shape_None,
	Shape_Square,
	Shape_Circle
};

struct SpriteRendererComponent
{
	static std::string Name() { return "SpriteRenderer"; }

	SpriteShape shape = Shape_None;
	glm::vec4 color = glm::vec4 { 1.0f };
};

static void DoSerialize(const SpriteRendererComponent& sprite, json& entityJson)
{
	entityJson["SpriteRenderer"]["shape"] = sprite.shape;

	json colorJson = json::array();
	colorJson.push_back(sprite.color.x);
	colorJson.push_back(sprite.color.y);
	colorJson.push_back(sprite.color.z);
	colorJson.push_back(sprite.color.w);
	entityJson["SpriteRenderer"]["color"] = colorJson;
}

static void DoDeserialize(SpriteRendererComponent& sprite, json& spriteRendererJson)
{
	auto& colorJson = spriteRendererJson["color"];
	glm::vec4 color { colorJson[0].get<float>(), colorJson[1].get<float>(), colorJson[2].get<float>(), colorJson[3].get<float>() };

	sprite.shape = spriteRendererJson["shape"].get<SpriteShape>();
	sprite.color = color;
}

static void RenderImGui(SpriteRendererComponent& spriteRenderer)
{
	const char* shapeTypeStrings[] = { "None", "Square", "Circle" };
	const char* currentShapeTypeString = shapeTypeStrings[(int)spriteRenderer.shape];
	if (ImGui::BeginCombo("Shape Type", currentShapeTypeString))
	{
		for (int i = 0; i < 3; i++)
		{
			bool isSelected = currentShapeTypeString == shapeTypeStrings[i];
			if (ImGui::Selectable(shapeTypeStrings[i], isSelected))
			{
				currentShapeTypeString = shapeTypeStrings[i];
				spriteRenderer.shape = (SpriteShape)i;
			}

			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}
}