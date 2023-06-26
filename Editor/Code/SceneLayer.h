#pragma once

#include "Core/Scene.h"

class SceneLayer
{
public:
	SceneLayer(Scene* scene);
	virtual ~SceneLayer();

	virtual void Render();

protected:
	Scene *m_scene;
};