#include <cassert>
#include <typeinfo>

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
		std::memset(mEntityToIndexMap, INVALID_INDEX, sizeof(size_t)*MAX_ENTITIES);
		std::memset(mIndexToEntityMap, INVALID_ENTITY, sizeof(Entity)*MAX_ENTITIES);
	}

	template<typename T>
	void ComponentArray<T>::insertData(Entity entity, const T &component)
	{
		assert(std::find(mEntityToIndexMap,
						 mEntityToIndexMap+MAX_ENTITIES,
						 entity) == mEntityToIndexMap+MAX_ENTITIES
						 && "Component added to same entity more than once.");

		size_t newIndex = mSize;
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

		size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
		size_t indexOfLastElement = mSize - 1;
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
		return mComponentArrays[typeName];
	}

	template <typename T>
	void ComponentManager::addComponent(Entity entity, const T &component)
	{
		GetComponentArray<T>()->insertData(entity, component);
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
}