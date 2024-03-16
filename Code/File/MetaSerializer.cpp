#include "MetaSerializer.h"
#include "Project/Project.h"
#include "Constants/GameEngine.h"

bool MetaSerializer::SerializeImage(Image& image, const std::filesystem::path & path)
{
	std::cout << "SerializeImage: " << path << std::endl;
	std::ofstream metaFile;

	metaFile.open(path.string() + META_FILE_EXTENSION);

	if (metaFile.is_open())
	{
		json imageJson;
		imageJson["ID"] = (uint64_t)image.GetID();
		imageJson["spriteMode"] = (int)image.m_spriteMode;

		json json_sprites = json::array();

		if (image.m_spriteMode == SpriteMode::Multiple)
		{
			int spriteWidth = image.m_texture->GetWidth() / image.m_cols;
			int spriteHeight = image.m_texture->GetHeight() / image.m_rows;

			int count = 0;
			for (int i = 0; i < image.m_rows; i++)
			{
				for (int j = 0; j < image.m_cols; j++)
				{
					json json_sprite;
					json_sprite["name"] = image.m_path.stem().string() + "_" + std::to_string(count++);
					json_sprite["x"] = spriteWidth * j;
					json_sprite["y"] = spriteHeight * i;
					json_sprite["width"] = spriteWidth;
					json_sprite["height"] = spriteHeight;
					json_sprites.push_back(json_sprite);
				}
			}
		}

		imageJson["sprites"] = json_sprites;

		metaFile << imageJson.dump(4);
	}
	else
	{
		std::cerr << "Error opening the file!" << std::endl;
		return false;
	}

	metaFile.close();

	return true;
}

bool MetaSerializer::DeserializeImage(Image& image, const std::filesystem::path& path)
{
	std::cout << "DeserializeImage\n" << std::endl;
	std::ifstream metaFile;

	metaFile.open(path.string() + META_FILE_EXTENSION);

	if (metaFile.is_open())
	{
		//ResourceManager* resourceManager = ResourceAPI::GetResourceManager();
		//resourceManager->RemoveSpriteAssets(image.m_spriteAssets);

		AssetManager& assetManager = Project::GetActive()->GetAssetManager();
		//assetManager.RemoveSpriteAssets(image.m_spriteAssets);
		assetManager.RemoveSprites(image.m_sprites);

		image.m_sprites.clear();

		json imageJson = json::parse(metaFile);

		if (imageJson["ID"].is_number_integer())
			image.SetID(imageJson["ID"].get<uint64_t>());

		image.m_spriteMode = (SpriteMode)imageJson["spriteMode"];

		if (image.m_spriteMode == SpriteMode::Multiple)
		{
			json json_sprites = imageJson["sprites"];

			for (auto& json_sprite : json_sprites)
			{
				Sprite sprite {
					json_sprite["name"],
					&image,
					json_sprite["x"],
					json_sprite["y"],
					json_sprite["width"],
					json_sprite["height"]
				};

				image.m_sprites.push_back(sprite);
			}
		}
		else if (image.m_spriteMode == SpriteMode::Single)
		{
			Sprite sprite {
				image.m_path.stem().string(),
				&image,
				0,
				0,
				(float)image.GetTexture()->GetWidth(),
				(float)image.GetTexture()->GetHeight()
			};

			image.m_sprites.push_back(sprite);
		}

		//resourceManager->AddSpriteAssets(image.m_spriteAssets);
		//assetManager.AddSpriteAssets(image.m_spriteAssets);
		assetManager.AddSprites(image.m_sprites);
	}
	else
	{
		std::cerr << "Error opening the file!" << std::endl;
		return false;
	}

	metaFile.close();

	return true;
}