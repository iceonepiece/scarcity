#pragma once

#include "Scene/Scene.h"
#include "Animations/Animation2D.h"
#include "Animations/AnimationState.h"
#include "FSM/EqualTransition.h"
#include "Systems/Collider2DSystem.h"
#include "Core/ResourceAPI.h"
#include "Core/LevelManager.h"
#include "Components/Components.h"
#include "../Player.h"
#include "Systems/AnimationSystem.h"
#include "Systems/PlayerSystem.h"
#include "Physics/PlayerFixtureData.h"

class MyLevel : public Scene
{
public:
	MyLevel() : Scene() {}

	virtual void Initialize() override
	{
		
		LevelManager::LoadLevel(this, "Levels/Level1.lua");
		
		Renderer& renderer = m_app->GetRenderer();
		renderer.SetCamera(m_camera.get());

		LuaEngine& luaEngine = m_app->GetLuaEngine();
		luaEngine.ReadScript("Scripts/sample.lua");
	
		/*
		ResourceAPI::LoadTexture("cat-idle", "Images/cat/Sprites-Idle-no shadow.png", true);
		ResourceAPI::LoadTexture("cat-walk", "Images/cat/Sprites-walk-no shadow .png", true);
		ResourceAPI::LoadTexture("cat-jump", "Images/cat/Sprites-jumping .png", true);
		*/
			
		auto player = m_manager.CreateEntity();
		b2Body* playerBody = m_physics.CreateBodyWithFixture({ 0, 5 }, { 0.85, 0.65 }, new PlayerFixtureData(player), true, false, PhysicsLayer::Layer_Player, { 0, -0.35 });
		player.AddComponent<TransformComponent>();
		player.AddComponent<Collider2DComponent>(playerBody);
		player.AddComponent<PlayerComponent>();
		player.AddComponent<ScriptableComponent>(new Player(player));

		m_camera->SetBody(player.GetComponent<Collider2DComponent>()->body);

		FiniteStateMachine* fsm = new FiniteStateMachine(player);
		player.AddComponent<SpriteAnimatorComponent>(fsm);

		AnimationState* idleState = new AnimationState({
			ResourceAPI::GetTexture("cat-idle"),
			0, 12, 2, 6, 0.08f, { 3.0f, 3.0f }
		});

		AnimationState* walkState = new AnimationState({
			ResourceAPI::GetTexture("cat-walk"),
			0, 11, 2, 6, 0.07f, { 3.0f, 3.0f }
		});

		AnimationState* jumpState = new AnimationState({
			ResourceAPI::GetTexture("cat-jump"),
			0, 8, 2, 5, 0.125f, { 3.0f, 3.0f }
		});


		FSMTransition* startWalk = new EqualTransition<bool>("walking", true);
		FSMTransition* endWalk = new EqualTransition<bool>("walking", false);
		FSMTransition* startJump = new EqualTransition<bool>("jumping", true);
		FSMTransition* endJump = new EqualTransition<bool>("jumping", false);

		FSMState* anyState = fsm->GetAnyState();
		anyState->AddTransition(startJump, jumpState);
		idleState->AddTransition(startWalk, walkState);
		walkState->AddTransition(endWalk, idleState);
		jumpState->AddTransition(endJump, idleState);

		fsm->AddState("Idle", idleState);
		fsm->AddState("Walk", walkState);
		fsm->AddState("Jump", jumpState);
		fsm->SetCurrentState(idleState);

		fsm->AddValue("jumping", new BoolValue(false));
		fsm->AddValue("walking", new BoolValue(false));

		m_systems.emplace_back(new AnimationSystem(this));
		m_systems.emplace_back(new PlayerSystem(this));
		m_systems.emplace_back(new Collider2DSystem(this));
	}
};