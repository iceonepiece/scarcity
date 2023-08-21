#pragma once

#include <tuple>

#include "AIComponent.h"
#include "BaseComponent.h"
#include "BoxCollider2DComponent.h"
#include "CameraComponent.h"
#include "CircleCollider2DComponent.h"
#include "Collider2DComponent.h"
#include "MockComponent.h"
#include "NativeScriptComponent.h"
#include "PlayerComponent.h"
#include "Rigidbody2DComponent.h"
#include "ScriptableComponent.h"
#include "SpriteAnimatorComponent.h"
#include "SpriteRendererComponent.h"
#include "TransformComponent.h"

using ComponentList = std::tuple<
	BaseComponent,
	BoxCollider2DComponent,
	CameraComponent,
	NativeScriptComponent,
	Rigidbody2DComponent,
	SpriteRendererComponent,
	TransformComponent
>;
