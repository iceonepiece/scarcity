#pragma once

#include "Core/Layer.h"
#include "EditorApplication.h"

class HubLayer : public Layer
{
public:
	HubLayer(EditorApplication& editor);
	virtual ~HubLayer() = default;

	virtual void Initialize() override;
	virtual void Shutdown() override;
	virtual void Update(float deltaTime) override;
	virtual void OnEvent(Event& event) override;
	virtual void RenderImGui() override;

private:
	EditorApplication& m_editor;
};