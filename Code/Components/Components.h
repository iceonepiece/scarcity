#pragma once

#include <tuple>

#include "AudioSourceComponent.h"
#include "BaseComponent.h"
#include "BoxCollider2DComponent.h"
#include "CameraComponent.h"
#include "CircleCollider2DComponent.h"
#include "Collider2DComponent.h"
#include "Collider2DGroupComponent.h"
#include "GridComponent.h"
#include "GroundDetectionComponent.h"
#include "IDComponent.h"
#include "LuaScriptComponent.h"
#include "MockComponent.h"
#include "NativeScriptComponent.h"
#include "PlayerComponent.h"
#include "Rigidbody2DComponent.h"
#include "SpriteAnimatorComponent.h"
#include "SpriteRendererComponent.h"
#include "TimerComponent.h"
#include "TransformComponent.h"
#include "UIComponents.h"
#include "ButtonComponent.h"

#define RequiredComponents BaseComponent, \
	TransformComponent

#define BasicComponents SpriteAnimatorComponent, \
	SpriteRendererComponent, \
	Rigidbody2DComponent, \
	BoxCollider2DComponent, \
	CircleCollider2DComponent, \
	Collider2DGroupComponent, \
	CameraComponent, \
	GridComponent, \
	GroundDetectionComponent, \
	NativeScriptComponent, \
	AudioSourceComponent, \
	LuaScriptComponent

#define UIComponents CanvasComponent, \
	ButtonComponent, \
	TextComponent


using CopyToSceneComponents = std::tuple<
	IDComponent,
	RequiredComponents,
	BasicComponents,
	UIComponents
>;

using ComponentList = std::tuple<
	RequiredComponents,
	BasicComponents,
	UIComponents
>;

using ToAddComponents = std::tuple<
	BasicComponents
>;

using ToAddUIComponents = std::tuple<
	UIComponents
>;
