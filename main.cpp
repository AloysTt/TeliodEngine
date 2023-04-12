#include <game/GameApplication.h>

int main(int argc, char **argv)
{
	GameApplication app;
	app.init();
	app.run();
	app.destroy();
}
