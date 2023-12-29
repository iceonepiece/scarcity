#pragma once

#include "ImGui_AssetWrapper.h"
#include "Graphics/Image.h"
#include "File/MetaSerializer.h"
#include "Project/Project.h"

class ImGui_ImageWrapper : public ImGui_AssetWrapper
{
public:
    virtual void SetAsset(Asset* asset) override
    {
        m_image = dynamic_cast<Image*>(asset);
    }

    virtual void RenderInspector() override;
    virtual void RenderBrowser(EditorLayer& editor) override;

private:
    Image* m_image;
};