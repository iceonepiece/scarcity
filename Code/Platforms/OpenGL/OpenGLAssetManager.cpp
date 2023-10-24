#include "OpenGLAssetManager.h"
#include "OpenGLTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture* OpenGLAssetManager::LoadTexture(const std::string& name, const char* filename, bool alpha)
{
    OpenGLTexture* texture = new OpenGLTexture();

    if (texture->Generate(filename, alpha))
    {
        m_textures.emplace(name, std::unique_ptr<OpenGLTexture>(texture));
        return m_textures[name].get();
    }

    delete texture;
    return nullptr;
}