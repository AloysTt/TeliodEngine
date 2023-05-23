#ifndef TELIOD_CORE_MESHRESOURCE_H
#define TELIOD_CORE_MESHRESOURCE_H

#include "ResourceManager.h"
#include <glm/vec3.hpp>

//predeclarations
namespace teliod::core
{
	class MeshResourceManager;
}

namespace teliod::core
{
	class MeshResource
	{
		DECLARE_RESOURCE(MeshResource, MeshResourceManager)
	public:
		const glm::vec3 * getVertices() const;
		const glm::vec3 * getNormals() const;
		const glm::vec3 * getUVs() const;
		int getNumVertices() const;

		const unsigned int * getTriangles() const;
		int getNumTriangles() const;
	private:
		glm::vec3 * vertices;
		glm::vec3 * normals;
		glm::vec3 * uvs;
		int numVertices;

		unsigned int * triangles;
		int numTriangles;
	};

	class MeshResourceManager : public ResourceManager<MeshResource>
	{
	public:
		static MeshResourceManager & getInstance();
		MeshResource * createResource() override;

	private:
		MeshResourceManager();
		~MeshResourceManager() override = default;
	};
}

#endif //TELIOD_CORE_MESHRESOURCE_H
