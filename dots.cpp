#include <SDL2/SDL.h>
#include <stdio.h>
#include <vector>
#include "BigPoint.h"

const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 300;

bool init();

void close();

SDL_Window* Window = NULL;

SDL_Renderer* Renderer = NULL;

bool init()
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO))
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	
	//Create window
	Window = SDL_CreateWindow("lines", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!Window)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	//Create renderer for window
	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED );
	if (!Renderer)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	return true;
}

void close()
{
	//Destroy window
	SDL_DestroyWindow(Window);
	Window = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if (init())
	{	
	    int wait = 5;	//actually inverted speed

		int startX = SCREEN_WIDTH/2;
		int startY = SCREEN_HEIGHT/2;
		BigPoint bigPoint(startX, startY);
		Uint32 timeout = SDL_GetTicks() + wait;

		int dirX = 1;
		int dirY = 1;

		std::vector<SDL_Rect> blocks;
		for (int i = 0; i < 6; i++) {
			blocks.push_back({20 + i*60, 75, 50, 30});
			blocks.push_back({40 + i*60, 115, 50, 30});
		}
		

        SDL_Event event; 
        bool quit = false;
        while (!quit)
		{
            while (SDL_PollEvent(&event)) 
                if (event.type == SDL_QUIT)
                    quit = true;  

			SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
			SDL_RenderClear(Renderer);
			
			SDL_SetRenderDrawColor(Renderer, 0x00, 0xFF, 0x00, SDL_ALPHA_OPAQUE);
			for (SDL_Rect r : blocks)
				SDL_RenderDrawRect(Renderer, &r);

			SDL_SetRenderDrawColor(Renderer, 0x00, 0x00, 0xAF, SDL_ALPHA_OPAQUE);
			SDL_RenderDrawPoints(Renderer, bigPoint.getPoints(), BigPoint::size);
			
			if (SDL_GetTicks() > timeout) 
			{
				timeout = SDL_GetTicks() + wait;
				bigPoint.move(dirX, dirY);

				// checking collisions with walls
				if (bigPoint.getCenter().y > SCREEN_HEIGHT - 3 ||
					bigPoint.getCenter().y < 2)
						dirY = -dirY;
				if (bigPoint.getCenter().x > SCREEN_WIDTH - 3 ||
					bigPoint.getCenter().x < 2)
						dirX = -dirX;

				for (std::vector<SDL_Rect>::iterator i = blocks.begin(); i != blocks.end(); i++)
					if (bigPoint.touchingRect(*i)) {
						dirX = -dirX;
						dirY = -dirY;
						blocks.erase(i);
						break;
				}

			}
			//Update screen
			SDL_RenderPresent(Renderer);
		}
    }

	//Free resources and close SDL
	close();

	return 0;
}
