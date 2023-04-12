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
	};

	class ShaderResourceManager : public core::ResourceManager<ShaderResource>
	{
	public:
		static ShaderResourceManager & getInstance();
		ShaderResource * createResource() override;

	private:
		ShaderResourceManager();
		~ShaderResourceManager() override = default;
	};
}

#endif // TELIOD_RENDER_SHADER_RESOURCE_H
