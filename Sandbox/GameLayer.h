#pragma once

#include <map>
#include "Core/Layer.h"
#include "Scene/Scene.h"

class GameLayer : public Layer
{
public:
	GameLayer();

	virtual void Initialize() override;
	virtual void Shutdown() override;

	virtual void Update(float dt) override;

private:
	std::map<std::string, std::unique_ptr<Scene>> m_sceneMap;
};