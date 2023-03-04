#pragma once

#include "LevelScene.h"
#include "../Core/SpriteAnimation.h"
#include "../Core/LevelManager.h"
#include "../Components/Collider2DComponent.h"
#include "../Components/CircleCollider2DComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/SpriteAnimatorComponent.h"
#include "../Components/TransformComponent.h"
#include "../Physics/PlayerFixtureData.h"
#include "../UIs/UIText.h"
#include "../UIs/UIList.h"
#include "../UIs/UIListItem.h"
#include "../UIs/UIContainer.h"
#include "../Animations/AnimationState.h"
#include "../FSM/EqualTransition.h"
#include "../FSM/AnimationDoneTransition.h"
#include "../Core/Value.h"
#include "../Systems/AnimationSystem.h"

class Level1 : public LevelScene
{
public:
	Level1(Application* game)
		: LevelScene(game)
	{
	}

	virtual void Init() override
	{
		std::cout << "Init Level1" << std::endl;

		ResourceManager::LoadTexture("heroKnight", "Assets/Images/HeroKnight.png", true);

		auto player = m_manager.CreateEntity();
		b2Body* playerBody = m_physics.CreateBodyWithFixture(b2Vec2{ 0, 8 }, b2Vec2{ 0.5, 1.2 }, new PlayerFixtureData(player), true, false, PhysicsLayer::Player);
		player.AddComponent<TransformComponent>();
		player.AddComponent<Collider2DComponent>(playerBody);
		player.AddComponent<PlayerComponent>();
	
		FiniteStateMachine* fsm = new FiniteStateMachine(player);
		player.AddComponent<SpriteAnimatorComponent>(fsm);

		AnimationState* idleState = new AnimationState({
			ResourceManager::s_textures["heroKnight"],
			{ { 0, 8 }, { 1, 8 }, { 2, 8 }, { 3, 8 }, { 4, 8 }, { 5, 8 }, { 6, 8 }, { 7, 8 } },
			glm::vec2{ 100, 55 },
			0.1,
			3.5
		});

		AnimationState* walkState = new AnimationState({
			ResourceManager::s_textures["heroKnight"],
			{ { 8, 8 }, { 9, 8 }, { 0, 7 }, { 1, 7 }, { 2, 7 }, { 3, 7 }, { 4, 7 }, { 5, 7 }, { 6, 7 }, { 7, 7 } },
			glm::vec2{ 100, 55 },
			0.1,
			3.5
		});

		AnimationState* attackState = new AnimationState({
			ResourceManager::s_textures["heroKnight"],
			{ { 0, 5 }, { 1, 5 }, { 2, 5 }, { 3, 5 }, { 4, 5 }, { 5, 5 } },
			glm::vec2{ 100, 55 },
			0.1,
			3.5
		});

		FSMTransition* startAttack = new EqualTransition<int>("attacking", 1);
		FSMTransition* endAttack = new AnimationDoneTransition();
		FSMTransition* startWalk = new EqualTransition<bool>("walking", true);
		FSMTransition* endWalk = new EqualTransition<bool>("walking", false);

		FSMState *anyState = fsm->GetAnyState();

		anyState->AddTransition(startAttack, attackState);
		idleState->AddTransition(startWalk, walkState);
		attackState->AddTransition(endAttack, idleState);
		walkState->AddTransition(endWalk, idleState);

		fsm->AddState("Idle", idleState);
		fsm->AddState("Attack", attackState);
		fsm->AddState("Walk", walkState);
		fsm->SetCurrentState(idleState);

		fsm->AddValue("attacking", new IntValue(0));
		fsm->AddValue("walking", new BoolValue(false));

		//auto circle = m_manager.CreateEntity();
		//b2Body* circleBody = m_physics.CreateCircleBody(b2Vec2{ 1, 8 }, 0.5, new FixtureData(circle, "ENEMY"));
		//circle.AddComponent<CircleCollider2DComponent>(circleBody);
		
		LevelManager::LoadLevel(this, "Assets/Levels/Level1.lua");

		m_camera.SetBody(player.GetComponent<Collider2DComponent>()->body);

		auto enemy = m_manager.CreateEntity();
		b2Body* enemyBody = m_physics.CreateBodyWithFixture(b2Vec2{ 1, 8 }, b2Vec2{ 0.5, 1.5 }, new FixtureData(enemy, "ENEMY"), true, false, PhysicsLayer::Enemy);
		enemy.AddComponent<TransformComponent>();
		enemy.AddComponent<Collider2DComponent>(enemyBody);
		enemy.AddComponent<AIComponent>(enemy);

		UIContainer* pausedMenuModal = new UIContainer(glm::vec2(0, 0), glm::vec2(400, 240), glm::vec4(1, 1, 1, 0.5));
		pausedMenuModal->SetAligment(UIAlignment::CENTER);
		m_ui.AddComponent(pausedMenuModal);

		PlayerSystem* playerSystem = new PlayerSystem(this);
		m_systems.emplace_back(playerSystem);

		AnimationSystem* animSystem = new AnimationSystem(this);
		m_systems.emplace_back(animSystem);

		m_gameStates.emplace("running", new RunningState(this));
		m_gameStates.emplace("paused", new PausedState(this, playerSystem, pausedMenuModal));

		ChangeGameState("running");
	}
};