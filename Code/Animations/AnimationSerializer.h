#pragma once

#include <filesystem>
#include <string>
#include <fstream>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "Core/FiniteStateMachine.h"
#include "Core/ResourceAPI.h"
#include "AnimationState.h"
#include "FSM/EqualTransition.h"

using json = nlohmann::json;

class AnimationSerializer
{
public:
	static FiniteStateMachine* DeserializeFSM(const std::filesystem::path& filepath, Entity entity)
	{
		FiniteStateMachine* fsm = nullptr;

		std::unordered_map<std::string, AnimationState*> stateMap;

		std::cout << "DeserializeFSM: " << filepath << std::endl;
		std::ifstream deserialzed(filepath);

		if (deserialzed.is_open())
		{
			fsm = new FiniteStateMachine(entity);
			json json_fsm = json::parse(deserialzed);

			json json_states = json_fsm["states"];

			for (auto& json_state : json_states)
			{
				if (!ResourceAPI::HasTexture(json_state["texture"]))
					continue;

				AnimationState* state = new AnimationState({
					ResourceAPI::GetTexture(json_state["texture"]),
					json_state["startFrame"],
					json_state["maxFrame"],
					json_state["rows"],
					json_state["cols"],
					json_state["frameSpeed"],
					{ json_state["scale"][0], json_state["scale"][1] }
				});

				if (state != nullptr)
				{
					std::cout << "Created state: " << json_state["name"] << std::endl;
					stateMap[json_state["name"]] = state;
					fsm->AddState(json_state["name"], state);
				}
			}

			fsm->SetCurrentState(stateMap[json_fsm["startState"]]);

			json json_transitions = json_fsm["transitions"];

			for (auto& json_transition : json_transitions)
			{
				if (json_transition["condition"].is_object())
				{
					FSMTransition* transition = nullptr;

					json json_condition = json_transition["condition"];
					if (json_condition["type"] == "Equal")
						transition = new EqualTransition<bool>(json_condition["variable"], json_condition["value"]);

					if (transition != nullptr)
					{
						FSMState* fromState = stateMap[json_transition["from"]];
						FSMState* toState = stateMap[json_transition["to"]];

						if (fromState && toState)
							fromState->AddTransition(transition, toState);
					}
				}

			}

			for (auto& json_variable : json_fsm["variables"])
			{
				if (json_variable["type"] == "bool")
				{
					fsm->AddValue(json_variable["name"], new BoolValue(json_variable["defaultValue"]));
				}
			}
		}
		else
		{
			std::cerr << "Error opening the file!" << std::endl;
			return nullptr;
		}

		deserialzed.close();

		return fsm;
	}
};