#pragma once

#include <filesystem>
#include <string>
#include "Core/UniqueID.h"

enum class AssetType
{
	None,
	Scene,
	NativeScript,
	Prefab,
	Image,
	Sprite,
	AnimatorController,
	AudioClip,
	AnimationClip,
	LuaScript,
	Font,
	Count
};

class Asset
{
public:
	Asset(const std::filesystem::path& path, AssetType type = AssetType::None)
		: m_type(type)
		, m_path(path)
	{}

	Asset(UniqueID id, const std::filesystem::path& path, AssetType type = AssetType::None)
		: m_ID(id)
		, m_type(type)
		, m_path(path)
	{}

	virtual ~Asset() = default;

	UniqueID GetID() { return m_ID; }
	void SetID(UniqueID id) { m_ID = id; }

	virtual std::string GetTypeString() { return "None"; }
	inline AssetType GetType() { return m_type; }

	inline void SetPath(const std::filesystem::path& path) { m_path = path; }
	inline std::filesystem::path GetPath() { return m_path; }

	inline std::string GetName() { return m_path.filename().string(); }

	friend class MetaSerializer;

protected:
	UniqueID m_ID;
	AssetType m_type;
	std::filesystem::path m_path;
};
