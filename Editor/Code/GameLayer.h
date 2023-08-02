#pragma once

#include "Core/Layer.h"
#include "EditorApplication.h"

class GameLayer : public Layer
{
public:
	GameLayer(EditorApplication& app);
	virtual void Update(float deltaTime) override;
	virtual void OnEvent(Event& event) override;
	virtual void Initialize() override;
	virtual void Shutdown() override;

private:
	EditorApplication& m_app;
};