#pragma once

#include "Asset.h"
#include <filesystem>
#include "Components/ComponentSerializer.h"
#include "Core/Entity.h"
#include "File/FileSystem.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class PrefabAsset : public Asset
{
public:
	PrefabAsset(const std::filesystem::path& path, Entity entity)
		: Asset(path, AssetType::Prefab)
		, m_entity(entity)
	{
		ComponentSerializer serializer(m_entity.GetRegistry());

		FileSystem::ReadFile(path, [&](std::fstream& fs)
		{
			json entityJson = json::parse(fs);

			std::apply([&](auto... componentTypes) {
				(serializer.Deserialize<decltype(componentTypes)>(entityJson, entity), ...);
			}, ComponentList{});
		});
	}

	virtual std::string GetTypeString() override { return "Prefab"; }
	Entity& GetEntity() { return m_entity; }

private:
	Entity m_entity;
};