namespace teliod::core
{

	template <typename T>
	ResourceManager<T>::ResourceManager()
	: mResources()
	{
	}

	template <typename T>
	ResourceManager<T>::~ResourceManager()
	{
		for (auto entry : mResources)
			delete entry.second;
	}

	template <typename T>
	T * ResourceManager<T>::getResource(const std::string &resName)
	{
		return mResources.at(resName);
	}
}
