#ifndef GAMEAPPLICATION_H
#define GAMEAPPLICATION_H

#include <core/Application.h>
#include <ECS/ECS.h>

class GameApplication : public teliod::core::Application
{
protected:
	void initInternal() override;

	void destroyInternal() override;

	void runInternal() override;
private:
	teliod::ecs::Entity torus;
};

#endif // GAMEAPPLICATION_H
