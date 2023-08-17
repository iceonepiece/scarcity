#pragma once

#include <unordered_map>
#include <functional>
#include <string>
#include "Scene/ScriptableEntity.h"

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