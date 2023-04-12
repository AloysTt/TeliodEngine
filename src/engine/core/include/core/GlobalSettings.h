#ifndef TELIOD_CORE_GLOBALSETTINGS_H
#define TELIOD_CORE_GLOBALSETTINGS_H

#include <string>
#include <unordered_map>

namespace teliod::core
{
	class GlobalSettings
	{
	public:
		static constexpr const char * RESOURCE_PATH{"resources"};

		inline static const std::string & getParam(const std::string & key)
		{
			static GlobalSettings instance;
			return instance.mSettings.at(key);
		}
	private:
		GlobalSettings();

		std::unordered_map<std::string, std::string> mSettings;
	};
}

#endif // TELIOD_CORE_GLOBALSETTINGS_H
