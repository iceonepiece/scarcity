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
		controllerJson["ID"] = (uint64_t)controller.GetID();

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


		json transitionsJson = json::array();

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

			auto& transitions = state->m_outgoingTransitions;

			for (auto& transition : transitions)
			{
				json transitionJson = json::object();

				transitionJson["fromState"] = state->m_name;
				transitionJson["nextState"] = "";
				if (transition->m_nextState != nullptr)
					transitionJson["nextState"] = transition->m_nextState->m_name;

				json conditionsJson = json::array();
				for (auto& condition : transition->m_conditions)
				{
					json conditionJson = json::object();
					conditionJson["mode"] = (int)condition.mode;
					conditionJson["parameter"] = condition.parameter.name;

					switch (condition.parameter.value.index())
					{
						case 0:
							conditionJson["threshold"] = std::get<float>(condition.parameter.value);
							break;

						case 1:
							conditionJson["threshold"] = std::get<int>(condition.parameter.value);
							break;

						case 2:
							conditionJson["threshold"] = std::get<bool>(condition.parameter.value);
							break;

						case 3:
							conditionJson["threshold"] = 0;
							break;
					}

					conditionsJson.push_back(conditionJson);
				}

				transitionJson["conditions"] = conditionsJson;
				transitionsJson.push_back(transitionJson);
			}

			statesJson.push_back(stateJson);
		}

		controllerJson["states"] = statesJson;
		controllerJson["transitions"] = transitionsJson;

		serialized << controllerJson.dump(2);

	}
}

void AnimationSerializer::Deserialize(AnimatorController& controller, const std::filesystem::path& filepath)
{
	std::cout << "Deserialize AnimatorController: " << filepath << std::endl;
	std::ifstream deserialzed(filepath);

	std::unordered_map<std::string, AnimatorState*> statesMap;

	if (deserialzed.is_open())
	{
		json controllerJson = json::parse(deserialzed);

		if (controllerJson["ID"].is_number_integer())
			controller.SetID(controllerJson["ID"].get<uint64_t>());

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

			AnimatorState& kState = *state;

			if (stateJson["motion"].is_number_unsigned())
			{
				uint64_t motionID = stateJson["motion"].get<uint64_t>();
				Application::Get().GetAssetManager().AddAssetLink(
					[&](Asset* asset)
					{
						if (AnimationClip* clip = dynamic_cast<AnimationClip*>(asset))
						{
							kState.m_motion = clip;
						}
					}
					, motionID
				);
			}

			controller.AddState(state);
			statesMap[name] = state;
		}

		json transitionsJson = controllerJson["transitions"];
		for (auto& transitionJson : transitionsJson)
		{
			std::string fromState = transitionJson["fromState"].get<std::string>();
			std::string nextState = transitionJson["nextState"].get<std::string>();

			if (statesMap.find(fromState) == statesMap.end() || statesMap.find(nextState) == statesMap.end())
				continue;

			AnimatorTransition* transition = new AnimatorTransition(statesMap[fromState], statesMap[nextState]);

			json conditionsJson = transitionJson["conditions"];
			for (auto& conditionJson : conditionsJson)
			{
				AnimatorCondition condition;
				condition.mode = (ConditionMode)conditionJson["mode"].get<int>();
				condition.parameter.name = conditionJson["parameter"].get<std::string>();

				if (AnimatorParameter* p = controller.GetParameter(condition.parameter.name))
				{
					ParameterType value = conditionJson["threshold"].get<float>();

					if (p->value.index() == 1)
						value = conditionJson["threshold"].get<int>();
					else if (p->value.index() == 2)
						value = conditionJson["threshold"].get<bool>();
					else if (p->value.index() == 3)
						value = Trigger{};

					condition.parameter.value = value;

					transition->AddCondition(condition);
				}
			}
		}
	}
	else
	{
		std::cerr << "Error opening the file!" << std::endl;
		return;
	}

	deserialzed.close();
}

void AnimationSerializer::Serialize(AnimationClip& clip, const std::filesystem::path& filePath)
{
	std::ofstream serialized(filePath);

	if (serialized.is_open())
	{
		json clipJson;

		clipJson["ID"] = (uint64_t)clip.GetID();
		clipJson["imageID"] = clip.m_image != nullptr ? (uint64_t)clip.m_image->GetID() : 0;

		json spriteIndicesJson = json::array();

		for (auto& spriteIndex : clip.m_spriteIndices)
			spriteIndicesJson.push_back(spriteIndex);

		clipJson["spriteIndices"] = spriteIndicesJson;

		serialized << clipJson.dump(2);
	}
}

void AnimationSerializer::Deserialize(AnimationClip& clip, const std::filesystem::path& filepath)
{
	std::ifstream deserialzed(filepath);

	if (deserialzed.is_open())
	{
		json clipJson = json::parse(deserialzed);

		if (clipJson["ID"].is_number_integer())
			clip.SetID(clipJson["ID"].get<uint64_t>());

		uint64_t imageID = clipJson["imageID"].get<uint64_t>();
		clip.m_image = (Image*)Application::Get().GetAssetManager().GetAssetByID(imageID);

		Application::Get().GetAssetManager().AddAssetLink(
			[&](Asset* asset)
			{
				clip.m_image = dynamic_cast<Image*>(asset);
			}
			, imageID
		);

		json spriteIndicesJson = clipJson["spriteIndices"];

		for (auto& spriteIndexJson : spriteIndicesJson)
			clip.m_spriteIndices.push_back(spriteIndexJson.get<size_t>());
	}
	else
	{
		std::cerr << "Error opening the file!" << std::endl;
		return;
	}
}
