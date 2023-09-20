#pragma once

#include <tuple>

#include "AIComponent.h"
#include "BaseComponent.h"
#include "BoxCollider2DComponent.h"
#include "CameraComponent.h"
#include "CircleCollider2DComponent.h"
#include "Collider2DComponent.h"
#include "GroundDetectionComponent.h"
#include "MockComponent.h"
#include "NativeScriptComponent.h"
#include "PlayerComponent.h"
#include "Rigidbody2DComponent.h"
#include "ScriptableComponent.h"
#include "SpriteAnimatorComponent.h"
#include "SpriteRendererComponent.h"
#include "TimerComponent.h"
#include "TransformComponent.h"

using ComponentList = std::tuple<
	BaseComponent,
	TransformComponent,
	SpriteAnimatorComponent,
	SpriteRendererComponent,
	BoxCollider2DComponent,
	CameraComponent,
	GroundDetectionComponent,
	NativeScriptComponent,
	Rigidbody2DComponent
>;
