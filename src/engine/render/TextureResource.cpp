#include <render/TextureResource.h>
#include <core/GlobalSettings.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <render/stb_image.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace teliod::render
{
    //
    // Manager

    TextureResourceManager::TextureResourceManager()
            : core::ResourceManager<TextureResource>()
    {
        // (will do conditional compilation)
        //
        // get all files in resources dir
        std::string pathString {core::GlobalSettings::getParam(core::GlobalSettings::RESOURCE_PATH) + "/textures"};
        std::filesystem::path p{pathString};

        std::filesystem::directory_iterator it{p};

        for (auto & file : it)
        {
            if (file.is_directory())
                continue;

            std::string base_filename = file.path().string().substr(file.path().string().find_last_of("/\\") + 1);
            TextureResource * res = new TextureResource();

            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);

            int width = 0;
            int height = 0;
            int channels = 3;
            unsigned char* data = stbi_load(base_filename.c_str(), &width, &height, &channels, 3);
            if (!data)
            {
                stbi_image_free(data);
                throw std::runtime_error("Failed to load texture: " + base_filename);
            }

            //std::cout << path << " : " << channels << "\n";
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            res->rawTex = data;
            res->mTex = texture;
            res->w = width;
            res->h = height;

            mResources.emplace( base_filename, res );
        }
    }

    TextureResource * TextureResourceManager::createResource()
    {
        return nullptr;
    }

    TextureResourceManager & TextureResourceManager::getInstance()
    {
        static TextureResourceManager instance;
        return instance;
    }

    //
    // Resource

    TextureResource::TextureResource() : rawTex(NULL), mTex(0), w(0), h(0)
    {}

    TextureResource::~TextureResource()
    {
        delete rawTex;
        glDeleteTextures( 1, &mTex );
    }

    unsigned char* TextureResource::getRawTexture() const { return rawTex; }
    unsigned int TextureResource::getMTexture() const { return mTex; }
    unsigned int TextureResource::getW() const { return w; }
    unsigned int TextureResource::getH() const { return h; }
}
