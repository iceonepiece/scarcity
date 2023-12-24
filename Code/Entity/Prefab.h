#pragma once

#include "Asset/Asset.h"
#include <filesystem>
#include "Components/ComponentSerializer.h"
#include "Entity/Entity.h"
#include "File/FileSystem.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Prefab : public Asset
{
public:
	Prefab(const std::filesystem::path& path, Entity entity)
		: Asset(path, AssetType::Prefab)
		, m_entity(entity)
	{
		ComponentSerializer serializer(m_entity.GetRegistry());

		FileSystem::ReadFile(path, [&](std::fstream& fs)
		{
			json entityJson = json::parse(fs);

			std::apply([&](auto... componentTypes) {
				(serializer.Deserialize<decltype(componentTypes)>(entityJson, entity), ...);
			}, CopyToSceneComponents{});
		});
	}

	virtual std::string GetTypeString() override { return "Prefab"; }
	Entity& GetEntity() { return m_entity; }

private:
	Entity m_entity;
};