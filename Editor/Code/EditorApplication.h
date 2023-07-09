#pragma once

#include <vector>
#include "Core/Application.h"
#include "EditorScene.h"
#include "ImGuiLayer.h"
#include "SceneLayer.h"

class EditorApplication : public Application
{
public:
	EditorApplication();
	virtual ~EditorApplication();

	virtual void Initialize(std::string title, int width, int height) override;
	virtual void Run() override;
	virtual void OnEvent(Event* event) override;

protected:
	virtual void ProcessInput() override;
	virtual void Update() override;
	virtual void Render() override;

	ImGuiLayer m_imGuiLayer;
	SceneLayer m_sceneLayer;
};