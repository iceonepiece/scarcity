#pragma once
#include <nlohmann/json.hpp>
#include <imgui/imgui.h>
#include "Physics/FixtureData.h"

using json = nlohmann::json;

enum class BodyType
{
	Static,
	Dynamic,
	Kinematic
};

inline b2BodyType Rigidbody2DTypeToBox2DBody(BodyType bodyType)
{
	switch (bodyType)
	{
		case BodyType::Static:    return b2_staticBody;
		case BodyType::Dynamic:   return b2_dynamicBody;
		case BodyType::Kinematic: return b2_kinematicBody;
	}

	return b2_staticBody;
}

struct Rigidbody2DComponent
{
	static std::string Name() { return "Rigidbody2D"; }

	BodyType type = BodyType::Static;
	bool fixedRotation = false;
	float gravityScale = 1.0f;

	void* body = nullptr;
	FixtureData* fixtureData = nullptr;

	float GetVelocityX()
	{
		if (b2Body* b = (b2Body*)body)
			return b->GetLinearVelocity().x;

		return 0.0f;
	}

	float GetVerticalY()
	{
		if (b2Body* b = (b2Body*)body)
			return b->GetLinearVelocity().y;

		return 0.0f;
	}

	void SetVelocity(float x, float y)
	{
		if (b2Body* b = (b2Body*)body)
			b->SetLinearVelocity({ x,  y });
	}

	void ApplyImpulse(float x, float y)
	{
		if (b2Body* b = (b2Body*)body)
			b->ApplyLinearImpulse(b2Vec2(x, y), b->GetWorldCenter(), true);
	}
};

static void DoSerialize(const Rigidbody2DComponent& rb2d, json& entityJson)
{
	entityJson["Rigidbody2D"]["type"] = rb2d.type;
	entityJson["Rigidbody2D"]["fixedRotation"] = rb2d.fixedRotation;
	entityJson["Rigidbody2D"]["gravityScale"] = rb2d.gravityScale;
}

static void DoDeserialize(Rigidbody2DComponent& rb2d, json& rb2dJson)
{
	rb2d.type = rb2dJson["type"].get<BodyType>();
	rb2d.fixedRotation = rb2dJson["fixedRotation"].get<bool>();
	rb2d.gravityScale = rb2dJson["gravityScale"].get<float>();
}

static void RenderImGui(Rigidbody2DComponent& rb2d)
{
    const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
    const char* currentBodyTypeString = bodyTypeStrings[(int)rb2d.type];
    if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
    {
        for (int i = 0; i < 2; i++)
        {
            bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
            if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
            {
                currentBodyTypeString = bodyTypeStrings[i];
				rb2d.type = (BodyType)i;
            }

            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }

    ImGui::Checkbox("Fixed Rotation", &rb2d.fixedRotation);
	ImGui::InputFloat("Gravity Scale", &rb2d.gravityScale);
}