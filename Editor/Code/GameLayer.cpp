#include "GameLayer.h"
#include <iostream>

GameLayer::GameLayer(EditorApplication& app)
	: m_app(app)
{
	std::cout << "GameLayer Constructor()\n\n";
}

void GameLayer::Initialize()
{
	std::cout << "GameLayer Initialize()\n\n";
}

void GameLayer::Shutdown()
{
	std::cout << "GameLayer Shutdown()\n\n";
}

void GameLayer::Update(float deltaTime)
{

}

void GameLayer::OnEvent(Event& event)
{
	if (event.GetType() == EventType::MouseButtonPressed)
	{
		std::cout << "GameLayer MouseButtonPressed\n";
	}
}