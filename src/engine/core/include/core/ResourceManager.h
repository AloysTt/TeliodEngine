#ifndef TELIOD_CORE_RESOURCEMANAGER_H
#define TELIOD_CORE_RESOURCEMANAGER_H

#include <concepts>
#include <vector>
#include <unordered_map>
#include <string>

namespace teliod::core
{
	template <typename T>
	class ResourceManager
	{
	public:
		ResourceManager();

		virtual ~ResourceManager();
		virtual T * createResource() = 0;

		T * getResource(const std::string & resName);

		ResourceManager & operator=(const ResourceManager & other) = delete;
		ResourceManager & operator=(ResourceManager && other) = delete;
		ResourceManager(ResourceManager && other) = delete;
		ResourceManager(const ResourceManager & other) = delete;
	protected:
		std::unordered_map<std::string, T *> mResources;
	};
}

#include <core/ResourceManager.inl>

//
// Macros

#define DECLARE_RESOURCE(name, manager)					\
friend class teliod::core::ResourceManager<name>;		\
friend class manager;                   				\
private:                                				\
	name();                             				\
    ~name();

#endif // TELIOD_CORE_RESOURCEMANAGER_H
