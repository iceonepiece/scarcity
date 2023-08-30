#include "OpenGLTexture.h"

OpenGLTexture::OpenGLTexture()
    : m_width(0)
    , m_height(0)
{
    glGenTextures(1, &m_id);
}

OpenGLTexture::OpenGLTexture(const std::string& path)
{
    glGenTextures(1, &m_id);
    Generate(path.c_str());
}

bool OpenGLTexture::Generate(const char* filename, bool alpha)
{
    bool success = false;

    glBindTexture(GL_TEXTURE_2D, m_id); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);

    if (data)
    {
        unsigned int internalFormat = 0;
        unsigned int imageFormat = 0;

        if (nrChannels == 4)
        {
            internalFormat = GL_RGBA8;
            imageFormat = GL_RGBA;
        }
        else if (nrChannels == 3)
        {
            internalFormat = GL_RGB8;
            imageFormat = GL_RGB;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        m_width = width;
        m_height = height;
        success = true;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    return success;
}

OpenGLTexture::~OpenGLTexture()
{

}

void OpenGLTexture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, m_id);
}