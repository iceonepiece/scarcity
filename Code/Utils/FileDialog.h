#pragma once

#include <string>

class FileDialog
{
public:
	static std::string OpenFolder();
	static std::string OpenFile(const char* filter);
	static std::string SaveFileDialog(const char* filter);
	static void SetNativeWindow(void* nativeWindow);

private:
	inline static void* s_nativeWindow;
};