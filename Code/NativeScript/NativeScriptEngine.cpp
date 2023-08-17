#include "NativeScriptEngine.h"
#include <Windows.h>
#include <iostream>
#include "Utils/FileDialog.h"

NativeScriptEngine::~NativeScriptEngine()
{
    FreeLibrary((HMODULE)m_dllHandle);
}

ScriptableEntity* NativeScriptEngine::CreateInstance(const std::string& className)
{
    if (m_createInstance)
        return m_createInstance(className);

    return nullptr;
}

std::unique_ptr<ScriptableEntity> NativeScriptEngine::CopyInstance(const std::string& className)
{
    if (m_scriptableInstanceMap.find(className) != m_scriptableInstanceMap.end())
    {
        return std::unique_ptr<ScriptableEntity>(m_scriptableInstanceMap[className]->Copy());
        //return m_scriptableInstanceMap[className]->Clone<ScriptableEntity>();
    }

    return nullptr;
}

void NativeScriptEngine::InitializeScriptableEntities()
{
    ShutdownScriptableEntities();

    for (auto& className : m_classNames)
    {
        ScriptableEntity* entity = CreateInstance(className);
        if (entity)
        {
            std::cout << className << " success created\n";
            m_scriptableInstanceMap[className] = entity;
        }
    }
}

void NativeScriptEngine::ShutdownScriptableEntities()
{
    for (auto& [className, scriptableEntity] : m_scriptableInstanceMap)
        delete scriptableEntity;

    m_scriptableInstanceMap.clear();
}

bool NativeScriptEngine::LoadNativeScripts(std::filesystem::path dllPath, std::vector<std::string>& classNames)
{
    bool success = false;
    std::filesystem::path tempDllPath = std::filesystem::current_path() / dllPath.filename();

    if (m_dllHandle)
        FreeLibrary((HMODULE)m_dllHandle);

    if (FileUtils::CopyFile(dllPath, tempDllPath))
    {
        HMODULE dllHandle = LoadLibrary(tempDllPath.string().c_str());

        if (dllHandle)
        {
            RegisterClassesFunction registerClasses = reinterpret_cast<RegisterClassesFunction>(
                GetProcAddress(dllHandle, "RegisterClasses")
                );

            CreateInstanceFunction createInstance = reinterpret_cast<CreateInstanceFunction>(
                GetProcAddress(dllHandle, "CreateInstance")
                );

            if (registerClasses && createInstance)
            {
                std::cout << "Load function Success" << std::endl;
                success = true;
                registerClasses();
            }

            m_createInstance = createInstance;
            m_dllHandle = dllHandle;
        }
    }

    return success;
}