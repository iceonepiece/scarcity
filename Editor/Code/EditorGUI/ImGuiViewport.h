#pragma once

#include <memory>
#include "Graphics/Framebuffer.h"
#include "Graphics/Camera.h"

class ImGuiViewport
{
public:
    ImGuiViewport() = default;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;

private:
    std::unique_ptr<Framebuffer> m_framebuffer;
    std::unique_ptr<Camera> m_camera;
};