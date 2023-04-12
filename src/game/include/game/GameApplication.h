#ifndef GAMEAPPLICATION_H
#define GAMEAPPLICATION_H

#include <core/Application.h>

class GameApplication : public teliod::core::Application
{
protected:
	void initInternal() override;

	void destroyInternal() override;

	void runInternal() override;
};

#endif // GAMEAPPLICATION_H
