#include <cassert>
#include <typeinfo>
#include "ECS.h"


namespace teliod::ecs
{
	//
	// Definitions for ComponentArray

	template<typename T>
	ComponentArray<T>::ComponentArray()
	: mEntityToIndexMap(),
	mIndexToEntityMap(),
	mSize(0)
	{
		std::memset(mEntityToIndexMap, INVALID_INDEX, sizeof(IndexType)*MAX_ENTITIES);
		std::memset(mIndexToEntityMap, INVALID_ENTITY, sizeof(Entity)*MAX_ENTITIES);
	}

	template<typename T>
	void ComponentArray<T>::insertData(Entity entity, const T &component)
	{
		assert(std::find(mEntityToIndexMap,
						 mEntityToIndexMap+MAX_ENTITIES,
						 entity) == mEntityToIndexMap+MAX_ENTITIES
						 && "Component added to same entity more than once.");

		IndexType newIndex = mSize;
		mEntityToIndexMap[entity] = newIndex;
		mIndexToEntityMap[newIndex] = entity;
		mComponentArray[newIndex] = component;
		++mSize;
	}

	template<typename T>
	void ComponentArray<T>::removeData(Entity entity)
	{
		assert(std::find(mEntityToIndexMap,
						 mEntityToIndexMap+MAX_ENTITIES,
						 entity) != mEntityToIndexMap+MAX_ENTITIES
						 && "Removing non-existent component.");

		IndexType indexOfRemovedEntity = mEntityToIndexMap[entity];
		IndexType indexOfLastElement = mSize - 1;
		mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

		Entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
		mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
		mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

		mEntityToIndexMap[entity] = INVALID_INDEX;
		mIndexToEntityMap[indexOfLastElement] = INVALID_ENTITY;

		--mSize;
	}

	template<typename T>
	const T &ComponentArray<T>::getData(Entity entity) const
	{
		assert(std::find(mEntityToIndexMap,
						 mEntityToIndexMap+MAX_ENTITIES,
						 entity) != mEntityToIndexMap+MAX_ENTITIES
						 && "Retrieving non-existent component.");
		return mComponentArray[mEntityToIndexMap[entity]];
	}

	template<typename T>
	T &ComponentArray<T>::getData(Entity entity)
	{
		assert(std::find(mEntityToIndexMap,
						 mEntityToIndexMap+MAX_ENTITIES,
						 entity) != mEntityToIndexMap+MAX_ENTITIES
						 && "Retrieving non-existent component.");
		return mComponentArray[mEntityToIndexMap[entity]];
	}

	template<typename T>
	void ComponentArray<T>::entityDestroyed(Entity entity)
	{
		if (std::find(mEntityToIndexMap,
					  mEntityToIndexMap+MAX_ENTITIES,
					  entity) != mEntityToIndexMap+MAX_ENTITIES)
			removeData(entity);
	}


	//
	// Definitions for ComponentManager

	template <typename T>
	void ComponentManager::registerComponent()
	{
		const char* typeName = typeid(T).name();

		assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Registering component type more than once.");

		mComponentTypes.insert({typeName, mNextComponentType});
		mComponentArrays.insert({typeName, new ComponentArray<T>});
		++mNextComponentType;
	}

	template <typename T>
	ComponentType ComponentManager::getComponentType() const
	{
		const char* typeName = typeid(T).name();
		assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");
		return mComponentTypes.at(typeName);
	}

	template <typename T>
	ComponentArray<T> * ComponentManager::getComponentArray()
	{
		const char* typeName = typeid(T).name();
		assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");
		return static_cast<ComponentArray<T> *>(mComponentArrays[typeName]);
	}

	template <typename T>
	void ComponentManager::addComponent(Entity entity, const T &component)
	{
		getComponentArray<T>()->insertData(entity, component);
	}

	template <typename T>
	void ComponentManager::removeComponent(Entity entity)
	{
		getComponentArray<T>()->removeData(entity);
	}

	template <typename T>
	const T &ComponentManager::getComponent(Entity entity) const
	{
		return getComponentArray<T>()->getData(entity);
	}

	template <typename T>
	T &ComponentManager::getComponent(Entity entity)
	{
		return getComponentArray<T>()->getData(entity);
	}

	inline void ComponentManager::entityDestroyed(Entity entity)
	{
		for (auto const& pair : mComponentArrays)
		{
			auto const& component = pair.second;
			component->entityDestroyed(entity);
		}
	}


	//
	// Definitions for systems

	inline const std::set<Entity> &System::getEntities() const
	{
		return mEntities;
	}

	inline std::set<Entity> &System::getEntities()
	{
		return mEntities;
	}

	template <typename T>
	T * SystemManager::registerSystem()
	{
		const char* typeName = typeid(T).name();
		assert(mSystems.find(typeName) == mSystems.end() && "Registering system more than once.");

		System * system = new T;
		mSystems.insert({typeName, system});
		return static_cast<T*>(system);
	}

	template <typename T>
	void SystemManager::setSignature(const Signature & signature)
	{
		const char* typeName = typeid(T).name();
		assert(mSystems.find(typeName) != mSystems.end() && "System used before registered.");

		mSignatures.insert({typeName, signature});
	}

	inline void SystemManager::entityDestroyed(Entity entity)
	{
		for (const auto & pair : mSystems)
		{
			pair.second->getEntities().erase(entity);
		}
	}

	inline void SystemManager::entitySignatureChanged(Entity entity, const Signature & entitySignature)
	{
		for (auto const& pair : mSystems)
		{
			const char * type = pair.first;
			System * system = pair.second;
			const Signature & systemSignature = mSignatures[type];

			if ((entitySignature & systemSignature) == systemSignature)
			{
				system->getEntities().insert(entity);
			}
				// Entity signature does not match system signature - erase from set
			else
			{
				system->getEntities().erase(entity);
			}
		}

	}


	//
	// Definitions for World

	inline /*static*/ World &World::getInstance()
	{
		static World w;
		return w;
	}

	inline Entity World::createEntity()
	{
		return mEntityManager.createEntity();
	}

	inline void World::destroyEntity(Entity entity)
	{
		mEntityManager.destroyEntity(entity);
		mComponentManager.entityDestroyed(entity);
		mSystemManager.entityDestroyed(entity);
	}

	template <typename T>
	void World::registerComponent()
	{
		mComponentManager.registerComponent<T>();
	}

	template <typename T>
	void World::addComponent(Entity entity, const T &component)
	{
		mComponentManager.addComponent<T>(entity, component);

		Signature signature = mEntityManager.getSignature(entity);
		signature.set(mComponentManager.getComponentType<T>(), true);
		mEntityManager.setSignature(entity, signature);

		mSystemManager.entitySignatureChanged(entity, signature);
	}

	template <typename T>
	void World::removeComponent(Entity entity)
	{
		mComponentManager.removeComponent<T>(entity);

		Signature signature = mEntityManager.getSignature(entity);
		signature.set(mComponentManager.getComponentType<T>(), false);
		mEntityManager.setSignature(entity, signature);

		mSystemManager.entitySignatureChanged(entity, signature);
	}

	template <typename T>
	const T & World::getComponent(Entity entity) const
	{
		return mComponentManager.getComponent<T>(entity);
	}

	template <typename T>
	T & World::getComponent(Entity entity)
	{
		return mComponentManager.getComponent<T>(entity);
	}

	template <typename T>
	ComponentType World::getComponentType()
	{
		return mComponentManager.getComponentType<T>();
	}

	template <typename T>
	T * World::registerSystem()
	{
		return mSystemManager.registerSystem<T>();
	}

	template <typename T>
	void World::setSystemSignature(const Signature &signature)
	{
		mSystemManager.setSignature<T>(signature);
	}
}
