#pragma once

#include "../Core/Scene.h"
#include "../Core/GUI.h"

class EditorScene : public Scene
{
public:
	EditorScene(Game* game);
	virtual ~EditorScene();

	virtual void Init() override;
	virtual void ProcessInput() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;

protected:
	GUI m_gui;
};