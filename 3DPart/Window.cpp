#include "Window.h"
#include "Log.h"
#include <glew.h>

Window::Window(){}

bool Window::initialize()
{
	int sdlInitResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlInitResult != 0) {
		Log::error(LogCategory::Video, "Unable to initialize SDL");
		return false;
	}

	SDLWindow = SDL_CreateWindow("Parallaxes", 100, 100, width, height, SDL_WINDOW_OPENGL);
	if (!SDLWindow)
	{
		Log::error(LogCategory::System, "Failed to create window");
		return false;
	}
	return true;
}

void Window::close()
{
	SDL_DestroyWindow(SDLWindow);
}
