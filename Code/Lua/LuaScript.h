#pragma once

#include "Asset/Asset.h"

class LuaScript : public Asset
{
public:
	LuaScript(const std::filesystem::path& path);
};