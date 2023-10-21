#include "Renderer.h"
#include <iostream>
#include "Platforms\OpenGL\OpenGLRenderer.h"

void Renderer::SetCamera(Camera *camera)
{
    m_camera = camera;
}

std::unique_ptr<Renderer> Renderer::Create()
{
    return std::make_unique<OpenGLRenderer>();
}






