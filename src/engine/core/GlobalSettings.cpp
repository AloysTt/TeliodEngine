#include <core/GlobalSettings.h>

namespace teliod::core
{
	GlobalSettings::GlobalSettings()
	: mSettings()
	{
		// will do conditional compilation
		mSettings[RESOURCE_PATH] = "res/raw";
	}
}
