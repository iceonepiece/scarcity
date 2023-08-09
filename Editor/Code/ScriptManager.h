#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>

const char* premakeContent = R"(
    architecture "x86_64"

    configurations { "Debug", "Release" }

project "Native-Script"
    kind "SharedLib"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"

    files { "**.h", "**.cpp" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
)";

class ScriptManager
{
public:
    static void CreateVisualStudioFiles(std::filesystem::path location , std::string name)
	{
        std::ofstream premakeFile(location / "premake5.lua");

        if (premakeFile.is_open())
        {
            premakeFile << "workspace \"" + name + "\"\n";
            premakeFile << premakeContent;
        }
        else
        {
            std::cout << "Error opening file: " << location / "premake5.lue" << std::endl;
        }

        premakeFile.close();
	}
};