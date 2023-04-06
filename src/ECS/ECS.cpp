#include <ECS.h>

#include <cassert>

namespace teliod::ecs
{

	EntityManager::EntityManager()
	: mAvailableEntities(),
	mSignatures(),
	mLivingEntitiesCount(0)
	{
		for (Entity e=0; e<MAX_ENTITIES; ++e)
			mAvailableEntities.push(e);
	}

	Entity EntityManager::createEntity()
	{
		assert(mLivingEntitiesCount < MAX_ENTITIES && "Entity limit reached.");

		Entity entityID = mAvailableEntities.front();
		mAvailableEntities.pop();
		++mLivingEntitiesCount;

		return entityID;
	}

	void EntityManager::destroyEntity(Entity entity)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");
		mSignatures[entity].reset();

		mAvailableEntities.push(entity);
		--mLivingEntitiesCount;
	}

	void EntityManager::setSignature(Entity entity, Signature signature)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");
		mSignatures[entity] = signature;
	}

	const Signature & EntityManager::getSignature(Entity entity) const
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");
		return mSignatures[entity];
	}

	ComponentManager::ComponentManager()
	: mComponentTypes(),
	mComponentArrays(),
	mNextComponentType(0)
	{
	}

	ComponentManager::~ComponentManager()
	{
		for (std::unordered_map<const char *, IComponentArray *>::value_type & pArray : mComponentArrays)
			delete pArray.second;
	}
}
