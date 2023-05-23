#ifndef TELIOD_RENDER_SHADER_RESOURCE_H
#define TELIOD_RENDER_SHADER_RESOURCE_H

#include "core/ResourceManager.h"


namespace teliod::render
{
	// predeclaration
	class ShaderResourceManager;

	class ShaderResource
	{
		DECLARE_RESOURCE(ShaderResource, ShaderResourceManager)
	public:
		unsigned int getShaderProgram() const;
		const std::string & getType() const;
	private:
		unsigned int shaderProgram;
		std::string type;
	};

	class ShaderResourceManager : public core::ResourceManager<ShaderResource>
	{
	public:
		static ShaderResourceManager & getInstance();
		ShaderResource * createResource() override;

	private:
		ShaderResourceManager();
		~ShaderResourceManager() override = default;

		void checkCompileErrors(unsigned int shader, std::string type);
	};
}

#endif // TELIOD_RENDER_SHADER_RESOURCE_H
