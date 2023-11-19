#include "AnimationSerializer.h"
#include "AnimatorState.h"
#include "Core/Application.h"

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
