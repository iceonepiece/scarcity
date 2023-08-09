#pragma once

#include <fstream>
#include <filesystem>
#include <string>
#include <cstdio>
#include <Windows.h>
#include <commdlg.h>
#include <shlobj_core.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

class FileUtils
{
public:
	static bool RemoveFile(std::filesystem::path path)
	{
		return std::remove(path.string().c_str());
	}

	static bool FileExists(std::filesystem::path filePath)
	{
		std::ifstream file(filePath);
		return file.good();
	}

	static bool CreateFolder(std::filesystem::path directoryPath)
	{
		if (!std::filesystem::exists(directoryPath))
		{
			if (std::filesystem::create_directory(directoryPath))
			{
				std::cout << "Directory created: " << directoryPath << std::endl;
				return true;
			}
			else
			{
				std::cout << "Failed to create directory: " << directoryPath << std::endl;
				return false;
			}
		}
		
		std::cout << "Directory already exists: " << directoryPath << std::endl;
		return false;
	}

	static std::string OpenFolder(void* nativeWindow)
	{
		std::string folderPath;

		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		if (SUCCEEDED(hr))
		{
			IFileOpenDialog* pFileOpenDialog;
			hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileOpenDialog));

			if (SUCCEEDED(hr))
			{
				DWORD dwOptions;
				pFileOpenDialog->GetOptions(&dwOptions);
				pFileOpenDialog->SetOptions(dwOptions | FOS_PICKFOLDERS);

				hr = pFileOpenDialog->Show(glfwGetWin32Window((GLFWwindow*)nativeWindow));

				if (SUCCEEDED(hr))
				{
					IShellItem* pItem;
					hr = pFileOpenDialog->GetResult(&pItem);

					if (SUCCEEDED(hr))
					{
						PWSTR pszFolderPath;
						hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFolderPath);

						if (SUCCEEDED(hr))
						{
							// Convert wide-character string to string
							int bufferSize = WideCharToMultiByte(CP_UTF8, 0, pszFolderPath, -1, nullptr, 0, NULL, NULL);
							if (bufferSize > 0)
							{
								std::vector<char> buffer(bufferSize);
								WideCharToMultiByte(CP_UTF8, 0, pszFolderPath, -1, buffer.data(), bufferSize, NULL, NULL);
								folderPath = buffer.data();
							}

							CoTaskMemFree(pszFolderPath);
						}

						pItem->Release();
					}
				}

				pFileOpenDialog->Release();
			}

			CoUninitialize();
		}

		return folderPath;
	}

	static std::string OpenFile(const char* filter, void* nativeWindow)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)nativeWindow);
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (GetCurrentDirectoryA(256, currentDir))
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;

		return std::string();
	}

	static std::string SaveFileDialog(const char* filter, void* nativeWindow)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)nativeWindow);
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (GetCurrentDirectoryA(256, currentDir))
			ofn.lpstrInitialDir = currentDir;

		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetSaveFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;

		return std::string();
	}
};