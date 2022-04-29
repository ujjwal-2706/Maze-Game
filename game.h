#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>

using namespace std;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

const int SCREEN_WIDTH = 1720;
const int SCREEN_HEIGHT = 1000;

// This function will start and open our game window
bool initialize()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		window = SDL_CreateWindow( "Maze Game IIT", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( window == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( renderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

void destroy()
{
	//Destroy window	
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	window = NULL;
	renderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}


// This class will handle the different textures used in the game
class Textures
{
	private:
		SDL_Texture* texture;
		int width,height;
	public:

		Textures()
		{
			texture = NULL;
			width = 0;
			height = 0;
		}

		// This function will destroy the texture
		void free()
		{
			SDL_DestroyTexture(texture);
			texture = NULL;
		}

		// This function will load the texture from the file path
		bool loadTexture(string path)
		{
			//The final texture
			SDL_Texture* newTexture = NULL;

			//Load image at specified path
			SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
			if( loadedSurface == NULL )
			{
				printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
			}
			else
			{
				//Create texture from surface pixels
				newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
				if( newTexture == NULL )
				{
					printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
				}
				else
				{
					//Get image dimensions
					width = loadedSurface->w;
					height = loadedSurface->h;
				}

				//Get rid of old loaded surface
				SDL_FreeSurface( loadedSurface );
			}
			texture = newTexture;
			return texture != NULL;
		}

		// This function will render the texture at the corresponding rect with the start rect
		// full checks if the full texture needs to be rendered or a part of it(in case of background)
		void render(SDL_Rect* start, SDL_Rect* destination, bool full)
		{
			if(!full)
			{
				SDL_RenderCopy(renderer,texture,start,destination);
			}
			else
			{
				SDL_RenderCopy(renderer,texture,NULL,destination);
			}
		}

		SDL_Texture* getTexture()
		{
			return texture;
		}
};


class Player
{

	// Note that the player width and height is 40 pixel

	private:
		int motivation,stamina,health,coins;
		int x,y;
		int velX = 0;
		int velY = 0;
		Textures player_image;
	public:

		// This will make the player as L or Light (Game characters) and assign it random position
		Player(string path)
		{
			srand(time(0));
			player_image.loadTexture(path);
			motivation = 50;
			health = 50;
			stamina = 50;
			coins = 0;
			x = rand() % SCREEN_WIDTH;
			y = rand() % SCREEN_HEIGHT;
		}

		// This function will free the texture
		void free()
		{
			player_image.free();
		}
		
		// This function will display the player
		void render()
		{
			SDL_Rect destination = {x,y,40,40};
			SDL_RenderCopy(renderer,player_image.getTexture(),NULL,&destination);
		}

		// Handling the movement of player by changing its velocity 
		void handleEvent(SDL_Event event)
		{
			// If a key is pressed
			if(event.type == SDL_KEYDOWN && event.key.repeat == 0)
			{
				switch( event.key.keysym.sym)
				{
					case SDLK_UP: velY -= 10; break;
					case SDLK_DOWN: velY += 10; break;
					case SDLK_LEFT: velX -= 10; break;
					case SDLK_RIGHT: velX += 10; break;
				}
			}

			//If a key was released
			else if( event.type == SDL_KEYUP && event.key.repeat == 0 )
			{
				//Adjust the velocity
				switch( event.key.keysym.sym )
				{
					case SDLK_UP: velY += 10; break;
					case SDLK_DOWN: velY -= 10; break;
					case SDLK_LEFT: velX += 10; break;
					case SDLK_RIGHT: velX -= 10; break;
				}
			}
		}

		// This will move the player whenever this is called
		void move()
		{
			//Move the player left or right
    		x += velX;

    		//If the player went too far to the left or right
    		if( ( x < 0 ) || ( x + 40 > SCREEN_WIDTH ) )
    		{
        		//Move back
        		x -= velX;
    		}

    		//Move the player up or down
    		y += velY;

    		//If the player went too far up or down
    		if( ( y < 0 ) || ( y + 40 > SCREEN_HEIGHT ) )
			{
				//Move back
				y -= velY;
			}
		}

};

