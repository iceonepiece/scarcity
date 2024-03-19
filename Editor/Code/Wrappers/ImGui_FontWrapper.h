#pragma once

#include "ImGui_AssetWrapper.h"
#include "Graphics/Font.h"

class ImGui_FontWrapper : public ImGui_AssetWrapper
{
public:
    virtual ~ImGui_FontWrapper() = default;

    virtual void SetAsset(Asset* asset) override
    {
        m_font = dynamic_cast<Font*>(asset);
    }

    virtual void RenderInspector() override
    {
        std::string name = m_font->GetName() + " (Font)";

        ImGui::Text(name.c_str());
    }

    virtual void RenderBrowser(EditorLayer& editor) override;

private:
    Font* m_font;
};