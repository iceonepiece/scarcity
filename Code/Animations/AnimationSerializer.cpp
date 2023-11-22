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

		json parametersJson = json::array();

		for (auto& param : controller.m_parameters)
		{
			json paramJson = json::object();
			paramJson["name"] = param.name;
			paramJson["type"] = param.value.index();

			if (param.value.index() == 0)
				paramJson["defaultValue"] = std::get<float>(param.value);
			else if (param.value.index() == 1)
				paramJson["defaultValue"] = std::get<int>(param.value);
			else if (param.value.index() == 2)
				paramJson["defaultValue"] = std::get<bool>(param.value);
			else if (param.value.index() == 3)
				paramJson["defaultValue"] = 0;

			parametersJson.push_back(paramJson);
		}
		controllerJson["parameters"] = parametersJson;

		json statesJson = json::array();
		for (auto& state : controller.m_states)
		{
			json stateJson = json::object();
			stateJson["name"] = state->m_name;
			stateJson["position"] = { state->m_position.x, state->m_position.y };

			stateJson["motion"] = 0;
			if (state->m_motion != nullptr)
				stateJson["motion"] = (uint64_t)state->m_motion->GetID();

			stateJson["speed"] = state->m_speed;

			auto& transitions = state->m_transitions;

			for (auto& transition : transitions)
			{
				json transitionJson = json::object();
				transitionJson["nextState"] = "";
				if (transition->m_nextState != nullptr)
					transitionJson["nextState"] = transition->m_nextState->m_name;

				json conditionsJson = json::array();
				for (auto& condition : transition->m_conditions)
				{
					json conditionJson = json::object();
					conditionJson["mode"] = (int)condition.mode;
					conditionJson["parameter"] = condition.parameter;
					conditionJson["threshold"] = condition.threshold;

					conditionsJson.push_back(conditionJson);
				}

				transitionJson["conditions"] = conditionsJson;
				stateJson["transitions"].push_back(transitionJson);
			}

			statesJson.push_back(stateJson);
		}
		controllerJson["states"] = statesJson;

		serialized << controllerJson.dump(2);

	}
}

void AnimationSerializer::Deserialize(AnimatorController& controller, const std::filesystem::path& filepath)
{
	std::cout << "Deserialize AnimatorController: " << filepath << std::endl;
	std::ifstream deserialzed(filepath);

	if (deserialzed.is_open())
	{
		json controllerJson = json::parse(deserialzed);

		json parametersJson = controllerJson["parameters"];

		for (auto& paramJson : parametersJson)
		{
			std::string name = paramJson["name"].get<std::string>();
			size_t type = paramJson["type"].get<size_t>();
			ParameterType value;

			if (type == 0)
				value = paramJson["defaultValue"].get<float>();
			else if (type == 1)
				value = paramJson["defaultValue"].get<int>();
			else if (type == 2)
				value = paramJson["defaultValue"].get<bool>();
			else if (type == 3)
				value = Trigger{};

			controller.AddParameter(name, value);
		}

		json statesJson = controllerJson["states"];
		for (auto& stateJson : statesJson)
		{
			std::string name = stateJson["name"].get<std::string>();
			AnimatorState* state = new AnimatorState(name);

			state->m_position = { stateJson["position"][0], stateJson["position"][1] };
			state->m_speed = stateJson["speed"];
			state->m_motion = nullptr;

			controller.AddState(state);
		}
	}
	else
	{
		std::cerr << "Error opening the file!" << std::endl;
		return;
	}

	deserialzed.close();
}
