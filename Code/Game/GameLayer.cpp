#include "GameLayer.h"
#include <iostream>
#include "Scene/SceneManager.h"
#include "Lua/LuaEngine.h"
#include "NativeScript/NativeScriptEngine.h"
#include "Graphics/Renderer.h"

GameLayer::GameLayer(Application& app)
	: m_app(app)
{
	std::cout << "GameLayer Constructor()\n\n";
}

GameLayer::GameLayer(Application& app, std::unique_ptr<Project> project)
	: m_app(app)
	, m_activeProject(std::move(project))
{
	std::cout << "GameLayer Constructor()\n\n";

	m_app.GetTagManager().Deserialize(m_activeProject->GetDirectory() / "ProjectSettings" / "TagManager.asset");

	std::filesystem::path luaFilePath = m_activeProject->GetDirectory() / (m_activeProject->GetName() + ".lua");

	if (FileSystem::FileExists(luaFilePath))
		m_app.GetLuaEngine().ReadScript(luaFilePath.string());

	m_app.GetAssetManager().InitializeAssets(m_activeProject->GetDirectory());

	OpenScene(m_activeProject->GetStartScene());

	Start();
}

void GameLayer::Start()
{
	if (m_activeScene != nullptr)
	{
		m_activeScene->SetApplication(&m_app);
		m_activeScene->StartNativeScripts(m_app.GetNativeScriptEngine());
		m_activeScene->Start();

		AddScene(m_activeScene->m_name, m_activeScene);
		ChangeScene(m_activeScene->m_name);
	}
}

bool GameLayer::OpenScene(const std::filesystem::path& absolutePath)
{
	std::cout << "OpenScene: " << absolutePath << std::endl;
	bool success = true;

	std::unique_ptr<Scene> loadedScene = m_activeProject->LoadScene(absolutePath);
	m_activeScene = loadedScene.release();

	SceneManager::ResolveUniqueIDs(*m_activeScene);
	ReloadNativeScripts();

	//m_activeScene->SetApplication(&m_app);
	//m_activeScene->Initialize();

	return success;
}

void GameLayer::ReloadNativeScripts()
{
	std::filesystem::path projectPath = m_activeProject->GetDirectory();

	if (!std::filesystem::exists(projectPath / "Scripts"))
		return;

	std::vector<std::string> scriptClassNames;

	for (auto& directoryEntry : std::filesystem::directory_iterator(m_activeProject->GetDirectory() / "Scripts"))
	{
		const auto& path = directoryEntry.path();
		std::string filenameString = path.filename().string();

		if (!directoryEntry.is_directory() && path.extension() == ".h")
		{
			std::cout << "Reloaded class: " << path.stem().string() << std::endl;
			scriptClassNames.push_back(path.stem().string());
		}
	}

	NativeScriptEngine& nativeScriptEngine = m_app.GetNativeScriptEngine();

	nativeScriptEngine.SetClassNames(scriptClassNames);

	if (nativeScriptEngine.LoadNativeScripts(projectPath / "bin" / "Debug" / "Native-Script.dll", m_nativeClassNames))
	{
		std::cout << "Success: Load native scripts\n";
		nativeScriptEngine.InitializeScriptableEntities();
	}
	else
	{
		std::cout << "Error: Load native scripts\n";
	}


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
	if (m_onExit)
	{
		DestroyAllScenes();
		m_app.Close();
		return;
	}

	if (m_firstFrame)
	{
		m_firstFrame = false;
		return;
	}

	Application& app = Application::Get();
	app.GetRenderer().Clear({ 0.2f, 0.2f, 0.2f, 1.0f });

	if (m_sceneMap.find(m_currentSceneName) != m_sceneMap.end())
	{
		Scene* playingScene = m_sceneMap[m_currentSceneName];

		playingScene->Update(deltaTime);
		playingScene->SetViewportSize(1280, 720);
		playingScene->OnViewportResize();
		playingScene->Render();


	}
}

void GameLayer::OnEvent(Event& event)
{
	if (event.GetType() == EventType::WindowClose)
		m_onExit = true;
}

void GameLayer::ChangeScene(const std::string& name)
{
	std::cout << "GameLayer::ChangeScene() - " << name << std::endl;

	if (m_sceneMap.find(name) == m_sceneMap.end())
	{
		if (Scene* sceneBlueprint = Application::Get().GetAssetManager().GetScene(name))
		{
			std::unique_ptr<Scene> activeScene = SceneManager::LoadScene(sceneBlueprint->GetPath());

			if (activeScene != nullptr)
			{
				activeScene->SetApplication(&m_app);
				activeScene->Initialize();
				m_sceneMap.insert({ name, activeScene.release() });
				m_currentSceneName = name;

				return;
			}
		}

		std::cout << "Cannot find the scene " << name << "\t[ GameLayer::ChangeScene() ]\n";
		return;
	}

	m_currentSceneName = name;
}

void GameLayer::AddScene(const std::string& name, const std::filesystem::path& filePath)
{
	if (m_sceneMap.find(name) == m_sceneMap.end())
	{
		Scene* scene = LoadScene(filePath);

		if (scene != nullptr)
		{
			m_sceneMap.insert({ name, scene });
		}
	}
}

void GameLayer::AddScene(const std::string& name, Scene* scene)
{
	if (scene != nullptr)
	{
		m_sceneMap.insert({ name, scene });
	}
}

Scene* GameLayer::GetCurrentScene()
{
	if (m_sceneMap.find(m_currentSceneName) != m_sceneMap.end())
		return m_sceneMap[m_currentSceneName];

	return nullptr;
}

void GameLayer::DestroyAllScenes()
{
	for (auto& scene : m_sceneMap) {
		scene.second->Stop();
		delete scene.second;
	}

	m_sceneMap.clear();
}

Scene* GameLayer::LoadScene(const std::filesystem::path& filePath)
{
	return nullptr;
}