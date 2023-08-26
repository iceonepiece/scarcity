#include "FileDialog.h"
#include <vector>
#include <Windows.h>
#include <commdlg.h>
#include <shlobj_core.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

std::string FileDialog::OpenFolder()
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

			hr = pFileOpenDialog->Show(glfwGetWin32Window((GLFWwindow*)s_nativeWindow));

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

std::string FileDialog::OpenFile(const char* filter)
{
	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };
	CHAR currentDir[256] = { 0 };
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)s_nativeWindow);
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

std::string FileDialog::SaveFileDialog(const char* filter)
{
	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };
	CHAR currentDir[256] = { 0 };
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)s_nativeWindow);
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

void FileDialog::SetNativeWindow(void* nativeWindow)
{
	s_nativeWindow = nativeWindow;
}