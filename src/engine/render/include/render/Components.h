#ifndef TELIOD_RENDER_COMPONENTS_H
#define TELIOD_RENDER_COMPONENTS_H

#include <ECS/ECS.h>
#include <render/ShaderResource.h>
#include "core/MeshResource.h"
#include "render/TextureResource.h"

namespace teliod::render
{
	class MeshRenderer
	{
		friend class MeshRendererSystem;
	public:
		MeshRenderer(ecs::Entity _entity, ShaderResource * _res, TextureResource * texRes = nullptr);
		MeshRenderer();
		MeshRenderer(const MeshRenderer & other);
		MeshRenderer & operator=(const MeshRenderer & other);
		MeshRenderer(MeshRenderer && other);
		MeshRenderer & operator=(MeshRenderer && other);
		~MeshRenderer();


		unsigned int getVAO() const;
		const ShaderResource & getShaderResource() const;

	private:
		bool isInit;
		void init(core::MeshResource * meshRes, ShaderResource * shaderRes);
		void init(core::MeshResource * meshRes, ShaderResource * shaderRes, TextureResource * texRes);
		unsigned int vertexBuffer;
		unsigned int indexBuffer;
		unsigned int vao;
		TextureResource * textureResource;
		ShaderResource * shaderResource;

		unsigned int verticesSize;
		unsigned int normalsSize;
		unsigned int uvSize;
		unsigned int indicesSize;
		void createBuffers();
		void deleteBuffers();
		void copyBuffers(const MeshRenderer & other);
	};
}

#endif //TELIOD_RENDER_COMPONENTS_H
