#pragma once

#include "Core/Scene.h"
#include "Graphics/Renderer.h"
#include "Components/Collider2DComponent.h"

class EditorScene : public Scene
{
public:
	EditorScene(Application& editor);

	virtual void Initialize() override;
	virtual void Render() override;
};