#include "AnimationSerializer.h"
#include "Animations/AnimatorState.h"
#include "Animations/AnimatorController.h"
#include "Core/Application.h"

void AnimationSerializer::Serialize(AnimatorController& controller, const std::filesystem::path& filePath)
{
	std::ofstream serialized(filePath);

	// Serialize the AnimatorController to a json file
	if (serialized.is_open())
	{
		json controllerJson;
		controllerJson["defaultState"] = controller.m_defaultState != nullptr ? controller.m_defaultState->m_name : "";

		json statesJson = json::array();

		for (auto& state : controller.m_states)
		{
			json stateJson = json::object();
			stateJson["name"] = state.m_name;
			stateJson["motion"]	= (uint64_t)state.m_motion->GetID();
			stateJson["speed"] = state.m_speed;

			statesJson.push_back(stateJson);
		}

		/*
		for (auto& transition : controller)
		{
			for (auto& animatorTransition : transition.second)
			{
				json transitionJson = json::object();
				transitionJson["to"] = animatorTransition.GetNextStateName();
				
				json conditionsJson = json::object();

				for (auto& condition : animatorTransition.m_conditionsData)
				{
					json conditionJson = json::object();
					conditionJson["mode"] = (int)condition.mode;
					conditionJson["parameter"] = condition.parameter;
					conditionJson["threshold"] = condition.threshold;
				}
			}

		}
		*/

		json json_sprites = json::array();
	}
}

AnimatorController* AnimationSerializer::Deserialize(const std::filesystem::path& filepath)
{
	AnimatorController* animController = nullptr;

	std::cout << "Deserialize AnimatorController: " << filepath << std::endl;
	std::ifstream deserialzed(filepath);

	if (deserialzed.is_open())
	{
		AssetManager& assetManager = Application::Get().GetAssetManager();
		animController = new AnimatorController();
		json json_fsm = json::parse(deserialzed);

		json json_states = json_fsm["states"];

		for (auto& json_state : json_states)
		{
			//if (!ResourceAPI::HasTexture(json_state["texture"]))
			if (!assetManager.HasTexture(json_state["texture"]))
			{
				std::cout << "No texture: " << json_state["texture"] << std::endl;
				continue;
			}

			AnimatorState animState ({
				assetManager.GetTexturePtr(json_state["texture"]),
				json_state["startFrame"],
				json_state["maxFrame"],
				json_state["rows"],
				json_state["cols"],
				json_state["frameSpeed"],
				{ json_state["scale"][0], json_state["scale"][1] }
			});
			
			animController->AddState(json_state["name"], animState);
		}

		animController->SetCurrentStateName(json_fsm["startState"]);

		for (auto& json_variable : json_fsm["variables"])
		{
			if (json_variable["type"] == "bool")
			{
				animController->SetBool(json_variable["name"], json_variable["defaultValue"]);
			}
			else if (json_variable["type"] == "int")
			{
				animController->SetInt(json_variable["name"], json_variable["defaultValue"]);
			}
		}

		json json_transitions = json_fsm["transitions"];

		for (auto& json_transition : json_transitions)
		{
			if (json_transition["condition"].is_object())
			{
				AnimatorTransition transition;

				json json_condition = json_transition["condition"];
				if (json_condition["type"] == "Equal")
				{
					std::string key = json_condition["variable"];
					bool value = json_condition["value"];

					transition.AddCondition([key, value](AnimatorController& controller) {
						return controller.GetBool(key) == value;
					});
				}

				transition.SetNextStateName(json_transition["to"]);
				animController->AddTransition(json_transition["from"], transition);
			}

		}

	}
	else
	{
		std::cerr << "Error opening the file!" << std::endl;
		return nullptr;
	}

	deserialzed.close();

	return animController;
}
