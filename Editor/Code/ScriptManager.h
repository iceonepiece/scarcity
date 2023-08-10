#pragma once

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <filesystem>

const std::string PREMAKE_FILE_CONTENT = R"(
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

const std::string PREMAKE_FILE_NAME = "premake5.lua";
const std::string PREMAKE_COMMAND = "premake5 vs2022";

class ScriptManager
{
public:
    static void CreateVisualStudioFiles(std::filesystem::path location , std::string name)
	{
        std::ofstream premakeFile(location / PREMAKE_FILE_NAME);

        if (premakeFile.is_open())
        {
            premakeFile << "workspace \"" + name + "\"\n";
            premakeFile << PREMAKE_FILE_CONTENT;

            std::string cdCommand = "cd /d " + location.string();

            std::system((cdCommand + " && " + PREMAKE_COMMAND).c_str());
            FileUtils::RemoveFile(location / PREMAKE_FILE_NAME);
        }
        else
        {
            std::cout << "Error opening file: " << location / PREMAKE_FILE_NAME << std::endl;
        }

        premakeFile.close();
	}
};