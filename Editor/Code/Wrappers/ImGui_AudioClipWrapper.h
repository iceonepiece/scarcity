#pragma once

#include "ImGui_AssetWrapper.h"
#include "Audio/AudioClip.h"
#include "Audio/Audio.h"

class ImGui_AudioClipWrapper : public ImGui_AssetWrapper
{
public:
	virtual ~ImGui_AudioClipWrapper() = default;

    virtual void SetAsset(Asset* asset) override
    {
        m_audioClip = dynamic_cast<AudioClip*>(asset);
    }

	virtual void RenderInspector() override
	{
        std::string name = m_audioClip->GetName() + " (Audio)";

        ImGui::Text(name.c_str());

        std::string durationText = "Seconds: " + std::to_string(m_audioClip->GetDuration());
        ImGui::Text(durationText.c_str());
        if (ImGui::Button("Play"))
        {
            Audio::Get()->PlaySound(m_audioClip);
        }
	}

    virtual void RenderBrowser(EditorLayer& editor) override;

private:
    AudioClip* m_audioClip;
};