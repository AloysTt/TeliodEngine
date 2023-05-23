#include <render/Components.h>
#include <core/Components.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace teliod::render
{
	MeshRenderer::MeshRenderer(ecs::Entity _entity, ShaderResource * _res, TextureResource * texRes)
	: vertexBuffer(0)
	, indexBuffer(0)
	, vao(0)
	, verticesSize(0)
	, normalsSize(0)
	, uvSize(0)
	, indicesSize(0)
	, shaderResource(_res)
	, isInit(false)
	, textureResource(texRes)
	{
		ecs::World & w = ecs::World::getInstance();
		if (texRes == nullptr)
			init(w.getComponent<core::MeshComponent>(_entity).getMesh(), _res);
		else
			init(w.getComponent<core::MeshComponent>(_entity).getMesh(), _res, texRes);
	}

	MeshRenderer::MeshRenderer()
	: vertexBuffer(0)
	, indexBuffer(0)
	, vao(0)
	, verticesSize(0)
	, normalsSize(0)
	, uvSize(0)
	, indicesSize(0)
	, shaderResource(nullptr)
	, isInit(false)
	, textureResource(nullptr)
	{
	}

	MeshRenderer::MeshRenderer(const MeshRenderer & other)
	: MeshRenderer()
	{
		if (!other.isInit)
			return;

		shaderResource = other.shaderResource;
		textureResource = other.textureResource;

		createBuffers();
		copyBuffers(other);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)verticesSize);
		if (textureResource != nullptr)
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(verticesSize+normalsSize));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

		glBindVertexArray(0);

		isInit = true;
	}

	MeshRenderer & MeshRenderer::operator=(const MeshRenderer & other)
	{
		deleteBuffers();
		if (!other.isInit)
			return *this;

		shaderResource = other.shaderResource;
		textureResource = other.textureResource;

		createBuffers();
		copyBuffers(other);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)verticesSize);
		if (textureResource != nullptr)
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(verticesSize+normalsSize));


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

		glBindVertexArray(0);

		isInit = true;
		return *this;
	}

	MeshRenderer::MeshRenderer(MeshRenderer && other)
	: MeshRenderer()
	{
		std::swap(isInit, other.isInit);
		std::swap(vao, other.vao);
		std::swap(vertexBuffer, other.vertexBuffer);
		std::swap(indexBuffer, other.indexBuffer);
		std::swap(verticesSize, other.verticesSize);
		std::swap(normalsSize, other.normalsSize);
		std::swap(uvSize, other.uvSize);
		std::swap(indicesSize, other.indicesSize);
		std::swap(shaderResource, other.shaderResource);
		std::swap(textureResource, other.textureResource);
	}

	MeshRenderer & MeshRenderer::operator=(MeshRenderer &&other)
	{
		if (isInit)
			deleteBuffers();

		std::swap(isInit, other.isInit);
		std::swap(vao, other.vao);
		std::swap(vertexBuffer, other.vertexBuffer);
		std::swap(indexBuffer, other.indexBuffer);
		std::swap(verticesSize, other.verticesSize);
		std::swap(normalsSize, other.normalsSize);
		std::swap(uvSize, other.uvSize);
		std::swap(indicesSize, other.indicesSize);
		std::swap(shaderResource, other.shaderResource);
		std::swap(textureResource, other.textureResource);

		return *this;
	}

	MeshRenderer::~MeshRenderer()
	{
		deleteBuffers();
	}

	void MeshRenderer::init(core::MeshResource * meshRes, ShaderResource * shaderRes)
	{
		createBuffers();

		verticesSize = sizeof(float) * 3 * meshRes->getNumVertices();
		normalsSize = verticesSize;
		indicesSize = sizeof(unsigned int) * 3 * meshRes->getNumTriangles();

		glBindVertexArray(vao);
		// vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, verticesSize+normalsSize, nullptr, GL_STATIC_DRAW); // allocation
		glBufferSubData(GL_ARRAY_BUFFER, 0, verticesSize, meshRes->getVertices()); // give vertices data
		glBufferSubData(GL_ARRAY_BUFFER, verticesSize, normalsSize, meshRes->getNormals()); // give normals data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)verticesSize);

		// element array buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, meshRes->getTriangles(), GL_STATIC_DRAW); // give indices data

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);

		isInit = true;
	}

	void MeshRenderer::init(core::MeshResource * meshRes, ShaderResource * shaderRes, TextureResource * texRes)
	{
		createBuffers();

		verticesSize = sizeof(float) * 3 * meshRes->getNumVertices();
		normalsSize = verticesSize;
		uvSize = sizeof(float) * 3 * meshRes->getNumVertices();
		indicesSize = sizeof(unsigned int) * 3 * meshRes->getNumTriangles();

		glBindVertexArray(vao);
		// vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, verticesSize+normalsSize+uvSize, nullptr, GL_STATIC_DRAW); // allocation
		glBufferSubData(GL_ARRAY_BUFFER, 0, verticesSize, meshRes->getVertices()); // give vertices data
		glBufferSubData(GL_ARRAY_BUFFER, verticesSize, normalsSize, meshRes->getNormals()); // give normals data
		glBufferSubData(GL_ARRAY_BUFFER, verticesSize+normalsSize, uvSize, meshRes->getUVs()); // give UVs data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)verticesSize);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(verticesSize+normalsSize));

		// element array buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, meshRes->getTriangles(), GL_STATIC_DRAW); // give indices data

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);

		isInit = true;
	}

	unsigned int MeshRenderer::getVAO() const
	{
		return vao;
	}

	const ShaderResource & MeshRenderer::getShaderResource() const
	{
		return *shaderResource;
	}

	void MeshRenderer::createBuffers()
	{
		glGenBuffers(1, &vertexBuffer);
		glGenBuffers(1, &indexBuffer);
		glGenVertexArrays(1, &vao);
	}

	void MeshRenderer::deleteBuffers()
	{
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &indexBuffer);
		glDeleteVertexArrays(1, &vao);
		vertexBuffer = indexBuffer = vao = 0;
		isInit = false;
	}

	void MeshRenderer::copyBuffers(const MeshRenderer & other)
	{
		verticesSize = other.verticesSize;
		normalsSize = other.normalsSize;
		uvSize = other.uvSize;
		indicesSize = other.indicesSize;

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, verticesSize+normalsSize+uvSize, nullptr, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, nullptr, GL_STATIC_DRAW);

		glBindBuffer(GL_COPY_READ_BUFFER, other.vertexBuffer);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, 0, 0, verticesSize+normalsSize+uvSize);
		glBindBuffer(GL_COPY_READ_BUFFER, other.indexBuffer);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ELEMENT_ARRAY_BUFFER, 0, 0, indicesSize);
		glBindBuffer(GL_COPY_READ_BUFFER, 0);

		isInit = true;
	}
}
