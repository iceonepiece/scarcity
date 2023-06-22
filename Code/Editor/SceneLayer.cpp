#include "SceneLayer.h"

SceneLayer::SceneLayer(Scene *scene)
	: m_scene{ scene }
{
	m_scene->Initialize();
}

SceneLayer::~SceneLayer()
{
	delete m_scene;
}

void SceneLayer::Render()
{
	m_scene->Render();
}
