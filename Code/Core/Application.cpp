#include "Application.h"
#include "Timer.h"
#include "Layer.h"
#include "ImGui/ImGuiManager.h"
#include "Input/NewInput.h"
#include "Graphics/Renderer.h"
#include "Lua/LuaEngine.h"
#include "Audio/Audio.h"

Application* Application::s_instance = nullptr;

Application::Application(const ApplicationConfigs& configs)
	: m_configs(configs)
{
	s_instance = this;

	m_window = Window::Create(WindowConfigs(configs.name, configs.width, configs.height));
	m_renderer = Renderer::Create();
	m_renderer->Initialize();

	m_luaEngine = std::make_unique<LuaEngine>();

	m_audio = Audio::Create();
	m_audio->Initialize();

	m_assetManager = std::make_unique<AssetManager>();
}

Application::~Application()
{
	for (auto& layer : m_layers)
		layer->Shutdown();

	m_audio->Destroy();
}

void Application::AddLayer(std::unique_ptr<Layer> layer)
{
	layer->Initialize();
	m_layers.emplace_back(std::move(layer));
}

void Application::PopLayer()
{
	if (m_layers.size() == 0)
		return;

	const auto& it = m_layers.begin();
	it->get()->Shutdown();

	m_layers.erase(it);
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
		m_input->Poll();

		for (auto& layer : m_layers)
			layer->Update(Timer::GetDeltaTime());

		m_imguiManager->Begin();
		{
			for (auto& layer : m_layers)
				layer->RenderImGui();
		}
		m_imguiManager->End();

		m_window->Render();
	}
}

