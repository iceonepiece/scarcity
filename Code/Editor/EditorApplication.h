#pragma once

#include <vector>
#include "../Core/Application.h"
#include "../Scenes/EditorScene.h"
#include "ImGuiLayer.h"

class EditorApplication : public Application
{
public:
	virtual ~EditorApplication();

	virtual void Initialize() override;
	virtual void Run() override;

protected:
	virtual void ProcessInput() override;
	virtual void Update() override;
	virtual void Render() override;

	ImGuiLayer m_imGuiLayer;
};