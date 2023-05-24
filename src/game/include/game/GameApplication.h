#ifndef GAMEAPPLICATION_H
#define GAMEAPPLICATION_H

#include <core/Application.h>
#include <ECS/ECS.h>

class GameApplication : public teliod::core::Application
{
protected:
	void initInternal() override;

	void destroyInternal() override;

	void runInternal(float dt) override;
private:
	teliod::ecs::Entity car;
	teliod::ecs::Entity goal;
	bool collided;
	float timeSinceCollided;
	int score;
};

#endif // GAMEAPPLICATION_H
