workspace "solution-name"
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