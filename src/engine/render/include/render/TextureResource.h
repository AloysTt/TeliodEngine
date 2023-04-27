//
// Created by leo on 27/04/23.
//

#ifndef ENGINE_TEXTURERESOURCE_H
#define ENGINE_TEXTURERESOURCE_H

#include "core/ResourceManager.h"

namespace teliod::render
{
    // predeclaration
    class TextureResourceManager;

    class TextureResource
    {
        DECLARE_RESOURCE(TextureResource, TextureResourceManager)
    public:
        unsigned char* getRawTexture() const;
        unsigned int getMTexture() const;
        unsigned int getW() const;
        unsigned int getH() const;
    private:
        unsigned char* rawTex;
        unsigned int mTex;
        unsigned int w, h;
    };

    class TextureResourceManager : public core::ResourceManager<TextureResource>
    {
    public:
        static TextureResourceManager & getInstance();
        TextureResource * createResource() override;

    private:
        TextureResourceManager();
        ~TextureResourceManager() override = default;
    };
}

#endif //ENGINE_TEXTURERESOURCE_H
