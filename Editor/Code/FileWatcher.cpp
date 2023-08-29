#include "FileWatcher.h"
#include "File/FileSystem.h"

FileWatcher::FileWatcher(const std::filesystem::path& targetPath)
    : m_targetPath(targetPath)
{
    m_fileWatcher = std::make_unique<filewatch::FileWatch<std::string>>(
        m_targetPath.string(),
        [&](const std::string& path, const filewatch::Event change_type) {
            switch (change_type)
            {
                case filewatch::Event::added:       OnAdded(path);      break;
                case filewatch::Event::removed:     OnRemoved(path);    break;
                case filewatch::Event::modified:    OnModified(path);   break;
                case filewatch::Event::renamed_old: OnRenamedOld(path); break;
                case filewatch::Event::renamed_new: OnRenamedNew(path); break;
            }
        }
    );
}

void FileWatcher::OnAdded(const std::string& path)
{
    std::filesystem::path filePath = m_targetPath / path;
    std::cout << filePath << " was added to the directory.\n";

    if (FileSystem::IsImageFile(filePath))
        FileSystem::GenerateImageMetaFile(filePath);
}

void FileWatcher::OnRemoved(const std::string& path)
{
    std::cout << path << " was removed to the directory.\n";
}

void FileWatcher::OnModified(const std::string& path)
{
    std::cout << path << " was modified.\n";
}

void FileWatcher::OnRenamedOld(const std::string& path)
{
    std::cout << path << " was renamed and this is the old name.\n";
}

void FileWatcher::OnRenamedNew(const std::string& path)
{
    std::cout << path << " was renamed and this is the new name.\n";
}