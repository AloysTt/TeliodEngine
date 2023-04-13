#include <core/Components.h>
#include "render/Components.h"


namespace teliod::core
{
	MeshComponent::MeshComponent()
	: mesh(nullptr)
	{

	}

	MeshComponent::MeshComponent(MeshResource * res)
	: mesh(res)
	{
	}

	MeshResource * MeshComponent::getMesh() const
	{
		return mesh;
	}

	void MeshComponent::setMesh(MeshResource *res)
	{
		mesh = res;
	}
}
