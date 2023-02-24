#include "SceneLayer.h"

SceneLayer::SceneLayer(Scene *scene)
	: m_scene{ scene }
{
	m_scene->Init();
}

SceneLayer::~SceneLayer()
{
	delete m_scene;
}

void SceneLayer::Render()
{
	m_scene->Render();
}
