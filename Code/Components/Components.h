#pragma once

#include <tuple>

#include "AIComponent.h"
#include "AudioSourceComponent.h"
#include "BaseComponent.h"
#include "BoxCollider2DComponent.h"
#include "CameraComponent.h"
#include "CircleCollider2DComponent.h"
#include "Collider2DComponent.h"
#include "GridComponent.h"
#include "GroundDetectionComponent.h"
#include "IDComponent.h"
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

using CopyToSceneComponents = std::tuple<
	IDComponent,

	BaseComponent,
	TransformComponent,
	CanvasComponent,
	SpriteAnimatorComponent,
	SpriteRendererComponent,
	BoxCollider2DComponent,
	CircleCollider2DComponent,
	CameraComponent,
	GridComponent,
	GroundDetectionComponent,
	NativeScriptComponent,
	Rigidbody2DComponent,
	AudioSourceComponent,

	ButtonComponent,
	TextComponent
>;

using ComponentList = std::tuple<
	BaseComponent,
	TransformComponent,
	CanvasComponent,
	SpriteAnimatorComponent,
	SpriteRendererComponent,
	BoxCollider2DComponent,
	CircleCollider2DComponent,
	CameraComponent,
	GridComponent,
	GroundDetectionComponent,
	NativeScriptComponent,
	Rigidbody2DComponent,
	AudioSourceComponent,

	ButtonComponent,
	TextComponent
>;

using ToAddComponents = std::tuple<
	BaseComponent,
	TransformComponent,
	CanvasComponent,
	SpriteAnimatorComponent,
	SpriteRendererComponent,
	BoxCollider2DComponent,
	CircleCollider2DComponent,
	CameraComponent,
	GridComponent,
	GroundDetectionComponent,
	NativeScriptComponent,
	Rigidbody2DComponent,
	AudioSourceComponent
>;

using ToAddUIComponents = std::tuple<
	ButtonComponent,
	TextComponent
>;
