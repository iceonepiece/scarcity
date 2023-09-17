#pragma once

#include <filesystem>
#include <string>

enum class AssetType
{
	None,
	NativeScript,
	Prefab,
	Texture,
	Sprite,
	AnimatorController,
	Audio
};

class Asset
{
public:
	Asset(const std::filesystem::path& path, AssetType type = AssetType::None)
		: m_type(type)
		, m_path(path)
	{}

	virtual ~Asset() = default;

	virtual std::string GetTypeString() { return "None"; }
	inline AssetType GetType() { return m_type; }
	inline std::filesystem::path GetPath() { return m_path; }
	inline std::string GetName() { return m_path.filename().string(); }

	friend class MetaSerializer;

protected:
	AssetType m_type;
	std::filesystem::path m_path;
};
