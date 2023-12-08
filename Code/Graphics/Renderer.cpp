#include "Renderer.h"
#include <iostream>
#include "Platforms\OpenGL\OpenGLRenderer.h"

void Renderer::SetCamera(Camera *camera)
{
    m_camera = camera;

    m_viewMatrix = camera->GetViewMatrix();
    m_projectionMatrix = camera->GetProjectionMatrix();
    m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}

void Renderer::SetCamera(CameraComponent cameraComponent)
{
    m_cameraComponent = cameraComponent;
}

std::unique_ptr<Renderer> Renderer::Create()
{
    return std::make_unique<OpenGLRenderer>();
}






