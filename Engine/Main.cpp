#include <stdlib.h>
#include "Application.h"
#include "Globals.h"

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/lib/win32/SDL2.lib" )
#pragma comment( lib, "SDL/lib/win32/SDL2main.lib" )

enum main_states
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

int main(int argc, char ** argv)
{
	SDL_Log("Starting game '%s'...", TITLE);

	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;
	Application* App = NULL;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			SDL_Log("-------------- Application Creation --------------");
			App = new Application();
			state = MAIN_START;
			break;

		case MAIN_START:

			SDL_Log("-------------- Application Init --------------");
			if (App->Init() == false)
			{
				SDL_Log("Application Init exits with ERROR");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				SDL_Log("-------------- Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				SDL_Log("Application Update exits with ERROR");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
			break;

		case MAIN_FINISH:

			SDL_Log("-------------- Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				SDL_Log("Application CleanUp exits with ERROR");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}
	}

	delete App;
	SDL_Log("Exiting game '%s'...\n", TITLE);
	return main_return;
}