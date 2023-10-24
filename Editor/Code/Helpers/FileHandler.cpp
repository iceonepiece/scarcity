#include "FileHandler.h"
#include "File/FileSystem.h"
#include "Core/Application.h"

void FileHandler::OnFileEvent(FileEvent& event)
{
    std::cout << "FileHandler::OnFileEvent()\n";
    switch (event.type)
    {
        case filewatch::Event::added:
        {
            std::cout << "File Added at: " << event.path << std::endl;

            if (FileSystem::IsImageFile(event.path))
            {
                std::cout << "Handle Image File!!!\n";
                FileSystem::GenerateImageMetaFile(event.path);
                Application::Get().GetAssetManager().LoadTexture(event.path.string(), event.path.string().c_str(), true);
            }
        }
        break;

        case filewatch::Event::removed:
        {
            std::cout << "File Removed at: " << event.path << std::endl;
            Application::Get().GetAssetManager().RemoveTexture(event.path.string());
        }
        break;

        case filewatch::Event::modified:
        {
            std::cout << "File modified at: " << event.path << std::endl;
        }
        break;
    }
}