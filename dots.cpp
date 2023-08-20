#include <SDL2/SDL.h>
#include <stdio.h>

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
		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		return false;
	}

	//Create renderer for window
	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED );
	if (!Renderer)
	{
		printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
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

class BigPoint
{
public:
	static const int size = 11;
	
	BigPoint()
	{
		int c = 10;
		points[0]={c,c};
		for (int i = -1; i <= 1; i++)
		{	
			points[i+2] = {c+i,c-1};
			points[i+5] = {c+i,c+1};
		}
		for (int i = 1; i<=2; i++)
		{
			points[6+i] = {c+i,c};
			points[8+i] = {c-i,c};
		}
	}

	void move(int dirX, int dirY)
	{
		for (int i = 0; i<size; i++)
		{
			points[i].x += dirX;
			points[i].y += dirY;
		}
	}

	SDL_Point* getPoints()
	{
		return points;
	}

	SDL_Point getCenter()
	{
		return points[0];
	}
private:
	SDL_Point points[BigPoint::size];
};


int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if (init())
	{	
	    int wait = 10;
		BigPoint bigPoint;
		Uint32 timeout = SDL_GetTicks() + wait;

        SDL_Event event; 
        bool quit = false;
        while (!quit)
		{
            while (SDL_PollEvent(&event)) 
                if (event.type == SDL_QUIT)
                    quit = true;  

			SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
			SDL_RenderClear(Renderer);
			
			SDL_SetRenderDrawColor(Renderer, 0x00, 0x00, 0xAF, SDL_ALPHA_OPAQUE);
			SDL_RenderDrawPoints(Renderer, bigPoint.getPoints(), BigPoint::size);
			
			if (SDL_GetTicks() > timeout) {
				timeout = SDL_GetTicks() + wait;
				bigPoint.move(1,1);
				if (bigPoint.getCenter().y > SCREEN_HEIGHT - 4)
					bigPoint.move(-SCREEN_HEIGHT+10,-SCREEN_HEIGHT+10);
			}
			//Update screen
			SDL_RenderPresent(Renderer);
		}
    }

	//Free resources and close SDL
	close();

	return 0;
}
