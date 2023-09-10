#pragma once

#include "Asset.h"
#include "FSM/FiniteStateMachine.h"
#include "Animations/AnimatorController.h"
#include "Animations/AnimationSerializer.h"

class AnimatorControllerAsset : public Asset
{
public:
	AnimatorControllerAsset(const std::filesystem::path& path)
		: Asset(path, AssetType::AnimatorController)
	{
		//m_fsm = AnimationSerializer::DeserializeFSM(path);
		m_controller = AnimationSerializer::Deserialize(path);
	}

	virtual ~AnimatorControllerAsset()
	{
		//delete m_fsm;
		delete m_controller;
	}

	AnimatorController* GetController() { return m_controller; }

private:
	AnimatorController* m_controller;
	FiniteStateMachine* m_fsm;
};