#include "LuaScript.h"

LuaScript::LuaScript(const std::filesystem::path& path)
	: Asset(path, AssetType::LuaScript)
{
}