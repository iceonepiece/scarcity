#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <unordered_map>
#include "Entity/ScriptableEntity.h"

typedef void (*RegisterClassesFunction)();
typedef void (*StartGameFunction)(Application& app);
typedef ScriptableEntity* (*CreateInstanceFunction)(const std::string&);

class NativeScriptEngine
{
public:
    ~NativeScriptEngine();

    bool LoadNativeScripts(std::filesystem::path dllPath, std::vector<std::string>& classNames);
    ScriptableEntity* CreateInstance(const std::string& className);
    std::unique_ptr<ScriptableEntity> CopyInstance(const std::string& className);
    void InitializeScriptableEntities();
    void ShutdownScriptableEntities();

    void RunStartGameFunction()
    {
        if (m_startGameFunction != nullptr)
            m_startGameFunction(Application::Get());
    }

    inline void SetClassNames(std::vector<std::string> classNames)
    {
        m_classNames = classNames;
    }

private:
    void* m_dllHandle = nullptr;
    CreateInstanceFunction m_createInstance = nullptr;
    StartGameFunction m_startGameFunction = nullptr;

    std::vector<std::string> m_classNames;
    std::unordered_map<std::string, ScriptableEntity*> m_scriptableInstanceMap;
};