#pragma once

#include "Components/BaseComponent.h"
#include "Components/CameraComponent.h"
#include "Components/LuaScriptComponent.h"
#include "Components/TilemapComponent.h"

void RenderImGui(BaseComponent& base);
void RenderImGui(CameraComponent& camera);
void RenderImGui(LuaScriptComponent& script);
void RenderImGui(TilemapComponent& tilemap);
