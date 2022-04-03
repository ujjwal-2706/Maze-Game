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
// SDL_Window* gWindow = NULL;
	
// //The surface contained by the window
// SDL_Surface* gScreenSurface = NULL;

// //The image we will load and show on the screen
// SDL_Surface* gHelloWorld = NULL;

// //This function will basically start and initialize the sdl window
// bool init()
// {
// 	//Initialization flag
// 	bool success = true;

// 	//Initialize SDL
// 	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
// 	{
// 		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
// 		success = false;
// 	}
// 	else
// 	{
// 		//Create window
// 		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
// 		if( gWindow == NULL )
// 		{
// 			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
// 			success = false;
// 		}
// 		else
// 		{
// 			//Get window surface
// 			gScreenSurface = SDL_GetWindowSurface( gWindow );
// 		}
// 	}

// 	return success;
// }

// //This function will load the media file from the directory
// bool loadMedia()
// {
// 	//Loading success flag
// 	bool success = true;

// 	//Load splash image
// 	gHelloWorld = SDL_LoadBMP( "hello_world.bmp" );
// 	if( gHelloWorld == NULL )
// 	{
// 		printf( "Unable to load image %s! SDL Error: %s\n", "hello_world.bmp", SDL_GetError() );
// 		success = false;
// 	}

// 	return success;
// }


// // This function will just free the sdl space and shutdown
// // One important point that should be kept in mind is that when pointers are deallocated
// // assign them to null (reason not known)
// void close()
// {
// 	//Deallocate surface
// 	SDL_FreeSurface( gHelloWorld );
// 	gHelloWorld = NULL;

// 	//Destroy window
// 	SDL_DestroyWindow( gWindow );
// 	gWindow = NULL;

// 	//Quit SDL subsystems
// 	SDL_Quit();
// }

// int main( int argc, char* args[] )
// {
// 	//Start up SDL and create window
// 	if( !init() )
// 	{
// 		printf( "Failed to initialize!\n" );
// 	}
// 	else
// 	{
// 		//Load media
// 		if( !loadMedia() )
// 		{
// 			printf( "Failed to load media!\n" );
// 		}
// 		else
// 		{
// 			//Apply the image
// 			SDL_BlitSurface( gHelloWorld, NULL, gScreenSurface, NULL );
			
// 			//Update the surface
// 			SDL_UpdateWindowSurface( gWindow );

// 			//Wait two seconds
// 			SDL_Delay( 2000 );
// 		}
// 	}

// 	//Free resources and close SDL
// 	close();

// 	return 0;
// }






// ------------------------------------------------------------------------------------------
// Now we will see how to interact with the event handling i.e. take user commands and set our application

//The window we'll be rendering to
// SDL_Window* gWindow = NULL;
	
// //The surface contained by the window
// SDL_Surface* gScreenSurface = NULL;

// //The image we will load and show on the screen
// SDL_Surface* gXOut = NULL;

// bool init()
// {
// 	//Initialization flag
// 	bool success = true;

// 	//Initialize SDL
// 	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
// 	{
// 		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
// 		success = false;
// 	}
// 	else
// 	{
// 		//Create window
// 		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
// 		if( gWindow == NULL )
// 		{
// 			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
// 			success = false;
// 		}
// 		else
// 		{
// 			//Get window surface
// 			gScreenSurface = SDL_GetWindowSurface( gWindow );
// 		}
// 	}

// 	return success;
// }

// bool loadMedia()
// {
// 	//Loading success flag
// 	bool success = true;

// 	//Load splash image
// 	gXOut = SDL_LoadBMP( "x.bmp" );
// 	if( gXOut == NULL )
// 	{
// 		printf( "Unable to load image %s! SDL Error: %s\n", "x.bmp", SDL_GetError() );
// 		success = false;
// 	}

// 	return success;
// }

// void close()
// {
// 	//Deallocate surface
// 	SDL_FreeSurface( gXOut );
// 	gXOut = NULL;

// 	//Destroy window
// 	SDL_DestroyWindow( gWindow );
// 	gWindow = NULL;

// 	//Quit SDL subsystems
// 	SDL_Quit();
// }

// int main( int argc, char* args[] )
// {
// 	//Start up SDL and create window
// 	if( !init() )
// 	{
// 		printf( "Failed to initialize!\n" );
// 	}
// 	else
// 	{
// 		//Load media
// 		if( !loadMedia() )
// 		{
// 			printf( "Failed to load media!\n" );
// 		}
// 		else
// 		{			
// 			//Main loop flag
// 			bool quit = false;
// 			// Basically we are using this quit to determine how long our application will run
// 			// As soon as quit becomes true, we will come out of loop and close the application

// 			//Event handler
// 			//This event handler will contain queue of events which will be fetched using the SDL_PollEvent function
// 			SDL_Event e;

// 			//While application is running
// 			while( !quit )
// 			{
// 				//Handle events on queue
// 				// (it gives 0 when there are no events left to be handled)
// 				while( SDL_PollEvent( &e ) != 0 )
// 				{
// 					//User requests quit
// 					//This quit will happen when the user presses the cross button at upper right side					
// 					if( e.type == SDL_QUIT )
// 					{
// 						quit = true;
// 					}
// 				}

// 				//Apply the image
// 				SDL_BlitSurface( gXOut, NULL, gScreenSurface, NULL );
			
// 				//Update the surface
// 				SDL_UpdateWindowSurface( gWindow );
// 			}
// 		}
// 	}
// 	//Free resources and close SDL
// 	close();

// 	return 0;
// }





//----------------------------------------------------------------------------------
//Key press surfaces constants
enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};
//Loads individual image
SDL_Surface* loadSurface( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
	
//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The images that correspond to a keypress
SDL_Surface* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];

//Current displayed image
SDL_Surface* gCurrentSurface = NULL;

bool init()
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
		gWindow = SDL_CreateWindow( "SDL Tutorial-4", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
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

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load default surface
	gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] = loadSurface( "press.bmp" );
	if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] == NULL )
	{
		printf( "Failed to load default image!\n" );
		success = false;
	}

	//Load up surface
	gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] = loadSurface( "up.bmp" );
	if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] == NULL )
	{
		printf( "Failed to load up image!\n" );
		success = false;
	}

	//Load down surface
	gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] = loadSurface( "down.bmp" );
	if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] == NULL )
	{
		printf( "Failed to load down image!\n" );
		success = false;
	}

	//Load left surface
	gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] = loadSurface( "left.bmp" );
	if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] == NULL )
	{
		printf( "Failed to load left image!\n" );
		success = false;
	}

	//Load right surface
	gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] = loadSurface( "right.bmp" );
	if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] == NULL )
	{
		printf( "Failed to load right image!\n" );
		success = false;
	}

	return success;
}

void close()
{
	//Deallocate surfaces
	for( int i = 0; i < KEY_PRESS_SURFACE_TOTAL; ++i )
	{
		SDL_FreeSurface( gKeyPressSurfaces[ i ] );
		gKeyPressSurfaces[ i ] = NULL;
	}

	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

SDL_Surface* loadSurface( std::string path )
{
	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
	}

	return loadedSurface;
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
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//Set default current surface
			gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					//User presses a key
					else if( e.type == SDL_KEYDOWN )
					{
						//Select surfaces based on key press
						switch( e.key.keysym.sym )
						{
							case SDLK_UP:
							gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ];
							break;

							case SDLK_DOWN:
							gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ];
							break;

							case SDLK_LEFT:
							gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ];
							break;

							case SDLK_RIGHT:
							gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ];
							break;

							default:
							gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
							break;
						}
					}
				}

				//Apply the current image
				SDL_BlitSurface( gCurrentSurface, NULL, gScreenSurface, NULL );
			
				//Update the surface
				SDL_UpdateWindowSurface( gWindow );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}



