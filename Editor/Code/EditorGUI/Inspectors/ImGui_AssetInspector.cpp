#include "ImGui_AssetInspector.h"

#include <imgui/imgui.h>
#include "ImGui_AssetInspector.h"
#include "Graphics/Texture.h"
#include <iostream>
#include "File/MetaSerializer.h"
#include "Audio/AudioSource.h"
#include "Audio/Audio.h"
#include "Animations/AnimationClip.h"
#include "../ImGuiComponents/ImGuiComponentRenderer.h"
#include "../../Wrappers/ImGui_WrapperManager.h"

void ImGui_AssetInspector::Render(Asset* asset)
{
    ImGui_WrapperManager::GetWrapper(*asset)->RenderInspector();
}