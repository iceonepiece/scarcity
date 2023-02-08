#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <sol/sol.hpp>

#include "ResourceManager.h"
#include "Entity.h"
#include "EntityManager.h"
#include "../Input/Input.h"
#include "../Rendering/Renderer.h"
#include "../Rendering/FontSystem.h"
#include "Camera.h"
#include "ParticleSystem.h"
#include "Scene.h"
#include "../Events/Event.h"

class Game
{
public:
	Game(int width = 1280, int height = 720);
	~Game();

	void Init();
	void Run();
	void OnEvent(Event* e);

	GLFWwindow* GetWindow() { return m_window; }
	void ChangeScene(std::string sceneName);

private:
	GLFWwindow* m_window;

	struct WindowData
	{
		std::vector<Event*> events;
	};

	WindowData m_data;

	int m_width;
	int m_height;
	float m_screenRatio;

	std::string m_currentSceneName;
	std::map<std::string, Scene*> m_scenes;
	EntityManager m_manager;

	sol::state m_lua;

	void ProcessInput(float deltaTime);
	void Update(float deltaTime);
	void Render();

	friend class GUI;
	friend class GUIComponent;
};
