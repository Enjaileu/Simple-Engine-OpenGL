#include "Game.h"

int main(int argc, char** argv)
{
	bool isGameInit = Game::instance().Initialize();
	if (isGameInit)
	{
		Game::instance().Load();
		Game::instance().Loop();
		Game::instance().Unload();
	}
	Game::instance().Close();
	return 0;
}