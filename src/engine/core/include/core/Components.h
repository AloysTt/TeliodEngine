#ifndef TELIOD_CORE_COMPONENTS_H
#define TELIOD_CORE_COMPONENTS_H

#include <core/MeshResource.h>

namespace teliod::core
{
	class MeshComponent
	{
	public:
		explicit MeshComponent(MeshResource * res);
		MeshComponent();
		MeshResource * getMesh() const;
		void setMesh(MeshResource * res);
	private:
		MeshResource * mesh;
	};
}

#endif // TELIOD_CORE_COMPONENTS_H
