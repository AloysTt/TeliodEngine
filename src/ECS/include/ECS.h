#ifndef TELIOD_ECS_ECS_H
#define TELIOD_ECS_ECS_H

#include <cstdint>
#include <bitset>
#include <queue>
#include <unordered_map>
#include <set>

namespace teliod::ecs
{
	using Entity = std::uint32_t;
	constexpr Entity MAX_ENTITIES = 5000;

	using ComponentType = std::uint8_t;
	constexpr ComponentType MAX_COMPONENTS = 32;

	using Signature = std::bitset<MAX_COMPONENTS>;

	class EntityManager
	{
	public:
		EntityManager();
		~EntityManager() = default;

		Entity createEntity();
		void destroyEntity(Entity entity);

		void setSignature(Entity entity, Signature signature);
		const Signature & getSignature(Entity entity) const;

	private:
		std::queue<Entity> mAvailableEntities;
		Signature mSignatures[MAX_ENTITIES];
		Entity mLivingEntitiesCount;
	};


	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;
		virtual void entityDestroyed(Entity entity) = 0;
	};

	template <typename T>
	class ComponentArray : public IComponentArray
	{
	public:
		ComponentArray();

		void insertData(Entity entity, const T & component);
		void removeData(Entity entity);

		const T & getData(Entity entity) const;
		T & getData(Entity entity);

		void entityDestroyed(Entity entity) override;

	private:
		T mComponentArray[MAX_ENTITIES];
		size_t mEntityToIndexMap[MAX_ENTITIES];
		Entity mIndexToEntityMap[MAX_ENTITIES];
		size_t mSize;


		// static
		static constexpr size_t INVALID_INDEX = -1;
		static constexpr Entity INVALID_ENTITY = -1;
	};


	class ComponentManager
	{
	public:
		ComponentManager();
		~ComponentManager();

		template <typename T>
		void registerComponent();

		template <typename T>
		ComponentType getComponentType() const;

		template <typename T>
		void addComponent(Entity entity, const T & component);
		template <typename T>
		void removeComponent(Entity entity);

		template <typename T>
		const T & getComponent(Entity entity) const;
		template <typename T>
		T & getComponent(Entity entity);

		inline void entityDestroyed(Entity entity);
	private:
		std::unordered_map<const char *, ComponentType> mComponentTypes;
		std::unordered_map<const char *, IComponentArray *> mComponentArrays;

		ComponentType mNextComponentType;

		template <typename T>
		ComponentArray<T> * getComponentArray();
	};


	class System
	{
	public:
		inline const std::set<Entity> & getEntities() const;
		inline std::set<Entity> & getEntities();
	private:
		std::set<Entity> mEntities;
	};

	class SystemManager
	{
	public:
		template <typename T>
		T * registerSystem();

		template <typename T>
		void setSignature(const Signature & signature);

		inline void entityDestroyed(Entity entity);

		inline void entitySignatureChanged(Entity entity, const Signature & entitySignature);

	private:
		std::unordered_map<const char *, Signature> mSignatures{};
		std::unordered_map<const char *, System *> mSystems{};
	};

	class World
	{
	public:
		inline static World & getInstance();

		inline Entity createEntity();
		inline void destroyEntity(Entity entity);


		template <typename T>
		void registerComponent();

		template <typename T>
		void addComponent(Entity entity, const T & component);
		template <typename T>
		void removeComponent(Entity entity);

		template <typename T>
		T & getComponent(Entity entity);
		template <typename T>
		const T & getComponent(Entity entity) const;

		template <typename T>
		ComponentType getComponentType();


		template <typename T>
		T * registerSystem();

		template <typename T>
		void setSystemSignature(const Signature & signature);


		World & operator=(const World & other) = delete;
		World & operator=(World && other) = delete;
		World(World && other) = delete;
		World(const World & other) = delete;
	private:
		World() = default;
		~World() = default;


		ComponentManager mComponentManager;
		EntityManager mEntityManager;
		SystemManager mSystemManager;
	};
}

#include <ECS.inl>

#endif // TELIOD_ECS_ECS_H
