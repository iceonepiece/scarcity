#pragma once

#include <string>
#include <memory>
#include <functional>
#include "Events/Event.h"

class Application;

using EventCallbackFn = std::function<void(Event*)>;

enum class CursorShape
{
	Arrow,
	Hand
};

struct WindowConfigs
{
	std::string title = "Untitled";
	uint32_t width = 1280;
	uint32_t height = 720;
};

struct WindowData
{
	std::string title;
	unsigned int width;
	unsigned int height;
	unsigned int xOffset;
	unsigned int yOffset;
	EventCallbackFn EventCallback;
};

class Window
{
public:
	Window(Application* app, std::string title, int width = 1280, int height = 720)
		: m_app(app)
		, m_title(title)
		, m_width(width)
		, m_height(height)
		, m_ratio(width / (float)height)
	{}

	static std::unique_ptr<Window> Create(const WindowConfigs& configs = WindowConfigs());

	virtual void SetEventCallback(EventCallbackFn callback) = 0;

	virtual ~Window() = default;
	virtual void ProcessInput() = 0;
	virtual void PreRender() = 0;
	virtual void Render() = 0;
	virtual bool WindowShouldClose() = 0;

	inline uint32_t GetWidth() { return m_width; }
	inline uint32_t GetHeight() { return m_height; }

	inline WindowData GetWindowData() { return m_data; }
	virtual void* GetNativeWindow() const = 0;

	virtual void SetVSync(bool enabled) = 0;
	virtual bool IsVSync() const = 0;

	virtual void SetTitle(const std::string& title) = 0;
	virtual void SetCursorShape(CursorShape shape) = 0;

protected:

	WindowData m_data;

	Application* m_app;
	std::string m_title;
	int m_width;
	int m_height;
	float m_ratio;
};