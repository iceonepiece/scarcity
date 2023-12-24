#pragma once

#include <unordered_map>
#include <functional>
#include <string>
#include "Asset/Asset.h"
#include "Entity/ScriptableEntity.h"

class NativeScript : public Asset
{
public:
    NativeScript(const std::filesystem::path& path)
        : Asset(path, AssetType::NativeScript)
    {}
};

typedef std::function<ScriptableEntity* ()> FactoryFunction;
static std::unordered_map<std::string, FactoryFunction> classRegistry;

static void RegisterClass(const std::string& className, FactoryFunction factory) {
    classRegistry[className] = factory;
}

#ifdef NATIVE_SCRIPT_DLL
#define API_CLASS __declspec(dllexport)
#else
#define API_CLASS static
#endif

extern "C" {
    API_CLASS ScriptableEntity* CreateInstance(const std::string& className) {
        if (classRegistry.find(className) != classRegistry.end()) {
            return classRegistry[className]();
        }
        return nullptr;
    }
}

#define REGISTER_CLASS(className) \
    class Class##className##Registration { \
    public: \
        Class##className##Registration() { \
            RegisterClass(#className, []() { return new className; }); \
        } \
    }; \
    static Class##className##Registration className##RegistrationInstance;

#define EXPORT_CLASS(className) \
    virtual ScriptableEntity* Copy() { \
        return new className; \
    } \
    \
    inline static std::unordered_map<std::string, void (className::*)()> s_functions; \
    \
    void AddFunction(const std::string& name, void (className::* fn)()) { \
        s_functions[name] = fn; \
    } \
    \
    virtual void CallFunction(const std::string& name) { \
        auto it = s_functions.find(name); \
        if (it != s_functions.end()) \
            (this->*(it->second))(); \
        else \
            std::cout << "Function not found!" << std::endl; \
    } \
    virtual std::vector<std::string> GetFunctionNames() { \
        std::vector<std::string> keys; \
        for (const auto& pair : s_functions) \
            keys.push_back(pair.first); \
        return keys; \
    }

#define EXPORT_FUNCTION(name) AddFunction(#name, &name)