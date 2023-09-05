#include "ImGuiManager.h"
#include <IconsFontAwesome6.h>

ImGuiManager::ImGuiManager(Application& app)
	: m_app(app)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	float baseFontSize = 18.0f;
	//float iconFontSize = baseFontSize * 2.0f / 3.0f;
	float iconFontSize = baseFontSize * 1.0f;

	io.Fonts->AddFontFromFileTTF("Fonts/Xolonium-Regular.ttf", baseFontSize);

	// merge in icons from Font Awesome
	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.GlyphMinAdvanceX = iconFontSize;
	io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, iconFontSize, &icons_config, icons_ranges);

	ImGui::StyleColorsDark();

	GLFWwindow* window = static_cast<GLFWwindow*>(m_app.GetWindow().GetNativeWindow());
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

}

ImGuiManager::~ImGuiManager()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiManager::Begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::End()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}