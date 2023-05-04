#ifndef TELIOD_RENDER_SHADER_RESOURCE_H
#define TELIOD_RENDER_SHADER_RESOURCE_H

#include "core/ResourceManager.h"
#include <unordered_map>
#include <vector>

namespace teliod::render
{
	// predeclaration
	class ShaderResourceManager;

	class ShaderResource
	{
		DECLARE_RESOURCE(ShaderResource, ShaderResourceManager)
	public:
        void setUniform( bool is_in_vs, std::string& name, void* data );
		unsigned int getShaderProgram() const;
        void updateShader() const;
	private:
		unsigned int shaderProgram;
        struct Attr;
        std::vector<struct Attr> vertAttr;
        std::vector<struct Attr> fragAttr;
	};

    struct ShaderResource::Attr
    {
        std::string name;
        std::string type;
        void* data;
    };

	class ShaderResourceManager : public core::ResourceManager<ShaderResource>
	{
	public:
		static ShaderResourceManager & getInstance();
		ShaderResource * createResource() override;

	private:
		ShaderResourceManager();
		~ShaderResourceManager() override = default;

        static bool isIdentifier( std::string& str );
        static bool Add_Attr( std::vector<struct ShaderResource::Attr >& vec, std::string& Attr );
		void checkCompileErrors(unsigned int shader, std::string type);
	};
}

#endif // TELIOD_RENDER_SHADER_RESOURCE_H
