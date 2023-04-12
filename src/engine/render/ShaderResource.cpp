#include <render/ShaderResource.h>

namespace teliod::render
{
	//
	// Manager

	ShaderResourceManager::ShaderResourceManager()
	: core::ResourceManager<ShaderResource>()
	{
	}

	ShaderResource * ShaderResourceManager::createResource()
	{
		return nullptr;
	}

	ShaderResourceManager & ShaderResourceManager::getInstance()
	{
		static ShaderResourceManager instance;
		return instance;
	}


	//
	// Resource

	ShaderResource::ShaderResource()
	{
	}

	ShaderResource::~ShaderResource()
	{
	}
}
