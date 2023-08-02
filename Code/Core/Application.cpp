#include "Application.h"
#include "Timer.h"
#include "Layer.h"

Application::~Application()
{
	for (auto& layer : m_layers)
		layer->Shutdown();
}

void Application::AddLayer(std::unique_ptr<Layer> layer)
{
	layer->Initialize();
	m_layers.emplace_back(std::move(layer));
}

void Application::OnEvent(Event& event)
{
	if (event.GetType() == EventType::WindowClose)
	{
		m_running = false;
		return;
	}

	for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it)
	{
		if (event.handled)
			break;

		(*it)->OnEvent(event);
	}
}

void Application::Run()
{
	while (m_running)
	{
		Timer::Tick();

		m_window->ProcessInput();
		m_window->PreRender();

		for (auto& layer : m_layers)
			layer->Update(Timer::GetDeltaTime());

		m_window->Render();
	}
}

