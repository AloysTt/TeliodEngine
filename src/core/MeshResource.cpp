#include <core/MeshResource.h>
#include <core/GlobalSettings.h>
#include <filesystem>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace teliod::core
{
	//
	// Manager

	MeshResourceManager::MeshResourceManager()
	: ResourceManager<MeshResource>()
	{
		// (will do conditional compilation)
		//
		// get all files in resources dir
		std::string pathString {GlobalSettings::getParam(GlobalSettings::RESOURCE_PATH) + "/meshes"};
		std::filesystem::path p{pathString};

		std::filesystem::directory_iterator it{p};

		Assimp::Importer importer;
		for (auto & file : it)
		{
			const aiScene * scene = importer.ReadFile(
						file.path().string(),
						aiProcess_CalcTangentSpace       |
							aiProcess_Triangulate            |
						 	aiProcess_JoinIdenticalVertices  |
		  					aiProcess_SortByPType
		  	);

			if (nullptr == scene)
			{
				std::cerr << "File " << file.path().string() << " not recognized as mesh.";
				continue;
			}

			auto test = file.path().string();
			// get file name
			std::string base_filename = file.path().string().substr(file.path().string().find_last_of("/\\") + 1);
			int lastDot = base_filename.find_last_of('.');
			std::string file_without_extension = base_filename.substr(0, lastDot);

			// create resources
			MeshResource * res = new MeshResource();
			mResources.emplace(file_without_extension, res);

			aiMesh * pMesh = scene->mMeshes[0];

			res->numVertices = pMesh->mNumVertices;
			res->vertices = new glm::vec3[res->numVertices];
			std::memcpy(res->vertices, pMesh->mVertices, pMesh->mNumVertices*3*sizeof(float));

			res->normals = new glm::vec3[res->numVertices];
			std::memcpy(res->normals, pMesh->mNormals, pMesh->mNumVertices*3*sizeof(float));

			res->numTriangles = pMesh->mNumFaces;
			res->triangles = new unsigned int[pMesh->mNumFaces*3];
			for (int i=0; i<pMesh->mNumFaces; ++i)
			{
				res->triangles[3*i] = pMesh->mFaces[i].mIndices[0];
				res->triangles[3*i+1] = pMesh->mFaces[i].mIndices[1];
				res->triangles[3*i+2] = pMesh->mFaces[i].mIndices[2];
			}
		}
	}

	MeshResource * MeshResourceManager::createResource()
	{
		return nullptr;
	}

	MeshResourceManager & MeshResourceManager::getInstance()
	{
		static MeshResourceManager instance;
		return instance;
	}


	//
	// Resource

	MeshResource::MeshResource()
	: vertices(nullptr)
	{
	}

	MeshResource::~MeshResource()
	{
		if (vertices != nullptr)
			delete [] vertices;
	}

	const glm::vec3 * MeshResource::getVertices() const
	{
		return vertices;
	}

	int MeshResource::getNumVertices() const
	{
		return numVertices;
	}
}
