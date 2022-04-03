#include <SDL2/SDL.h>
#include <iostream>
using namespace std;

// These will be used for the screen dimension
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


// ------------This was the part to open up a window using sdl -----------------
// int main( int argc, char* args[] )
// {
// 	//The window we'll be rendering to
// 	SDL_Window* window = NULL;
	
// 	//The surface contained by the window
// 	SDL_Surface* screenSurface = NULL;
//     // Here since we gave no surface thats why initially it was giving white surface

// 	//Initialize SDL
// 	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
// 	{
// 		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
//         // GetError function is quite important when it comes to sdl since it gives us
//         // The error quite nicely
// 	}
// 	else
// 	{
// 		//Create window
// 		window = SDL_CreateWindow( "COP gaming tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
// 		// Here in the first heading we have the top bar
//         // 2nd means the location on the screen where sdl window pops up
//         // 3rd and 4th will mean the size of screen
//         // last to show the window and if createWindow gives an error then we will get Null in window
//         if( window == NULL )
// 		{
// 			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
// 		}
// 		else
// 		{
// 			//Get window surface
// 			screenSurface = SDL_GetWindowSurface( window );

// 			//Fill the surface white
// 			SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
			
// 			//Update the surface
// 			SDL_UpdateWindowSurface( window );

// 			//Wait two seconds
// 			SDL_Delay( 2000 );
//             // This delay function will maintain the screen for 2 seconds and during this time
//             // all hardware features can't be used
// 		}
// 	}

// 	//Destroy window
// 	SDL_DestroyWindow( window );
//     // Deallocate the space

// 	//Quit SDL subsystems
// 	SDL_Quit();

// 	return 0;
// }








// ---------------- This was the part to apply the image to the sdl window---------
//The window we'll be rendering to
SDL_Window* gWindow = NULL;
	
//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gHelloWorld = NULL;

//This function will basically start and initialize the sdl window
bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface( gWindow );
		}
	}

	return success;
}

//This function will load the media file from the directory
bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load splash image
	gHelloWorld = SDL_LoadBMP( "hello_world.bmp" );
	if( gHelloWorld == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", "hello_world.bmp", SDL_GetError() );
		success = false;
	}

	return success;
}


// This function will just free the sdl space and shutdown
// One important point that should be kept in mind is that when pointers are deallocated
// assign them to null (reason not known)
void close()
{
	//Deallocate surface
	SDL_FreeSurface( gHelloWorld );
	gHelloWorld = NULL;

	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Apply the image
			SDL_BlitSurface( gHelloWorld, NULL, gScreenSurface, NULL );
			
			//Update the surface
			SDL_UpdateWindowSurface( gWindow );

			//Wait two seconds
			SDL_Delay( 2000 );
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}




