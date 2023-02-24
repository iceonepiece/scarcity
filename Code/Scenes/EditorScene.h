#pragma once

#include "../Core/Scene.h"
#include "../Core/GUI.h"
#include "../Editor/EditorMouse.h"

class EditorScene : public Scene
{
public:
	EditorScene(Application* game);
	virtual ~EditorScene();

	virtual void Init() override;
	virtual void OnEvent(Event* e) override;
	virtual void ProcessInput() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;

protected:
	GUI m_gui;
	EditorMouse m_editorMouse;
};