#include "LuaScript.h"
#include "File/FileSystem.h"
#include <iostream>

LuaScript::LuaScript(const std::filesystem::path& path)
	: Asset(path, AssetType::LuaScript)
{
}