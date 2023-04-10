#include "core/MeshResource.h"

using namespace teliod;

int main(int argc, char **argv)
{
	auto & manager = core::MeshResourceManager::getInstance();
	auto val = manager.getResource("torus");
}
