#include "extras.h"

using namespace std;

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

				//Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}

				 //Initialize SDL_mixer
                if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                {
                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                    success = false;
                }
			}
		}
	}

	return success;
}

void destroy()
{
	// Free the audio
	Mix_FreeMusic(music1);
	Mix_FreeChunk(coinCollect);
	Mix_FreeChunk(treasureCollect);
	Mix_FreeChunk(yuluChange);
	Mix_FreeChunk(thanks);
	music1 = NULL;
	coinCollect = NULL;
	treasureCollect = NULL;
	yuluChange = NULL;
	thanks = NULL;

	//Destroy window	
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	window = NULL;
	renderer = NULL;

	//Quit SDL subsystems
	Mix_Quit();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

// The Player class will contain all the details of the player
class Player
{

	// Note that the player width and height is 40 pixel
	private:
		int motivation,stamina,health,coins;
		int x,y;
		Coordinates map;
		int velX = 0;
		int velY = 0;
		// this will determine the angle of rotation of player
		double angle = 0.0;
		Textures player_image;
		Textures playerYulu_image;
		bool onYulu = false;
		int rateYulu = 0;
	public:

		// This will make the player as L or Light (Game characters) and assign it random position
		Player(string path, string yuluPath)
		{
			map.x = 5;
			map.y = 5;
			srand(time(0));
			player_image.loadTexture(path);
			playerYulu_image.loadTexture(yuluPath);
			motivation = 50;
			health = 50;
			stamina = 50;
			coins = 0;
			x = rand() % (SCREEN_WIDTH - PLAYER_WIDTH);
			y = rand() % (SCREEN_HEIGHT - PLAYER_HEIGHT);
		}

		// This function will free the texture
		void free()
		{
			player_image.free();
			playerYulu_image.free();
		}

		// This function will give the texture of player meter to be displayed
		Textures displayMeter()
		{
			Textures meter;
			string meterText = "";
			meterText = meterText + "Coins : " + to_string(coins) + " ";
			meterText = meterText + "Stamina : " + to_string(stamina) + " ";
			meterText = meterText + "Health : " + to_string(health) + " ";
			meterText = meterText + "Motivation : " + to_string(motivation);
			//Open the font
			meterFont = TTF_OpenFont( "Fonts/ABeeZee-Regular.ttf", 28 );
			SDL_Color textColor = {0,0,0};
			if( meterFont == NULL )
			{
				printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
				return meter;
			}
			else
			{
				//Render text surface
				SDL_Surface* textSurface = TTF_RenderText_Solid( meterFont, meterText.c_str(), textColor );
				if( textSurface == NULL )
				{
					printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
					return meter;
				}
				else
				{
					SDL_Texture* display = SDL_CreateTextureFromSurface(renderer,textSurface);
					meter.setTexture(display);
					meter.setDimension(textSurface->w,textSurface->h);
					SDL_FreeSurface(textSurface);
					//global font
					TTF_CloseFont( meterFont );
					meterFont = NULL;
					return meter;
				}
			}
		}
		
		// This function will display the player
		void render()
		{
			if(!onYulu)
			{
				SDL_Rect destination = {x,y,PLAYER_WIDTH,PLAYER_HEIGHT};
				SDL_RenderCopyEx(renderer,player_image.getTexture(),NULL,&destination,angle,NULL,SDL_FLIP_NONE);
			}
			else
			{
				SDL_Rect destination = {x,y,PLAYER_WIDTH,PLAYER_HEIGHT};
				SDL_RenderCopyEx(renderer,playerYulu_image.getTexture(),NULL,&destination,angle,NULL,SDL_FLIP_NONE);
			}
			
		}

		// Handling the movement of player by changing its velocity 
		vector<int> handleEvent(SDL_Event event, vector<Coordinates> buildings,vector<Coordinates> yuluStand)
		{
			SDL_Rect player_pos = {x,y,PLAYER_WIDTH,PLAYER_HEIGHT};
			// If a key is pressed and no collision with building 
			if(event.type == SDL_KEYDOWN && event.key.repeat == 0)
			{
				switch( event.key.keysym.sym)
				{
					case SDLK_UP: velY -= 10; angle -= 90; break;
					case SDLK_DOWN: velY += 10; angle += 90;  break;
					case SDLK_LEFT: velX -= 10; break;
					case SDLK_RIGHT: velX += 10; break;
					case SDLK_0:
						if(building_collision(player_pos,buildings).x != -1)
						{
							Mix_PlayChannel(-1,thanks,0);
							vector<int> result;
							result.push_back(0);
							coins -= 10;
							health  += 50;
							return result;
						}

						else if(yulu_collision(player_pos,yuluStand).x != -1)
						{
							if(!onYulu)
							{
								Mix_PlayChannel(-1,yuluChange,0);
							}
							onYulu = true;
						}
						break;
					case SDLK_1:
						vector<int> result;
						result.push_back(1);
						if(yulu_collision(player_pos,yuluStand).x != -1)
						{
							if(onYulu)
							{
								Mix_PlayChannel(-1,yuluChange,0);
							}
							onYulu = false;	
						}
						return result;
						break;

				}
			}

			//If a key was released
			else if( event.type == SDL_KEYUP && event.key.repeat == 0 )
			{
				//Adjust the velocity
				switch( event.key.keysym.sym )
				{
					case SDLK_UP: velY += 10; angle += 90; break;
					case SDLK_DOWN: velY -= 10; angle -= 90; break;
					case SDLK_LEFT: velX += 10; break;
					case SDLK_RIGHT: velX -= 10; break;
				}
			}
			vector<int> temp;
			return temp;
		}

		Coordinates getMap()
		{
			return map;
		}
		// This will move the player whenever this is called
		vector<vector<int>> move(vector<Coordinates> coinList,vector<Coordinates> boxes)
		{
			if(onYulu)
			{
				rateYulu += 1;
				rateYulu %= 300;
				coins -= rateYulu / 299;
			}
			//Move the player left or right
    		x += velX;
			if(onYulu)
			{
				x += velX;
			}

			if( ( x < 0 ) && map.y > 0)
			{
				x = SCREEN_WIDTH - PLAYER_WIDTH - 5;
				map.y -= 1;
			}

			if( x < 0 && map.y == 0)
			{
				x -= 2 * velX;
			}

			if( ( x + PLAYER_WIDTH > SCREEN_WIDTH ) && map.y < 10 )
			{
				x = 0;
				map.y +=1;
			}

			if( ( x + PLAYER_WIDTH > SCREEN_WIDTH ) && map.y == 10)
			{
				x -= 2 * velX;
			}


    		//Move the player up or down
    		y += velY;

			if(onYulu)
			{
				y += velY;
			}

			if( ( y < 0 ) && map.x > 0)
			{
				y = SCREEN_HEIGHT - PLAYER_HEIGHT - 5;
				map.x -= 1;
			}

			if( y< 0 && map.x == 0)
			{
				y -= 2 * velY;
			}

			if( ( y + PLAYER_HEIGHT > SCREEN_HEIGHT ) && map.y < 7 )
			{
				y = 0;
				map.x +=1;
			}

			if( ( y + PLAYER_HEIGHT > SCREEN_HEIGHT )  && map.y == 7)
			{
				y -= 2 * velY;
			}


			SDL_Rect player_rect = {x,y,PLAYER_WIDTH,PLAYER_HEIGHT};
			vector<vector<int>> meterIncrease;
			meterIncrease =  checkAllCollisions(player_rect,coinList,boxes);
			coins += meterIncrease[0][0];
			int box = meterIncrease[0][1];
			for(int i =0; i < box; i++)
			{
				vector<int> value = gift();
				motivation += value[0];
				stamina += value[1];
				health += value[2];
				coins += value[3];
			}
			// we have now catched the coins and boxes collected in meterIncrease vector
			return meterIncrease;
			// For the treasure box will do later
		}
};



class BackGround
{
	private:
		vector<vector<Textures>> background_images;
		Coordinates map;
	public:
		BackGround()
		{
			map.x = 5;
			map.y = 5;
			for(int i =0 ; i < 8;i++)
			{
				vector<Textures> temp;
				for(int j = 0; j < 11;j++)
				{
					string fileName = "Grid/row-" + to_string(i+1) + "-column-" + to_string(j+1) + ".png";
					Textures back;
					back.loadTexture(fileName);
					temp.push_back(back);
				}
				background_images.push_back(temp);
			}
		}

		Textures getBackGround(int i, int j)
		{
			return background_images[i][j];
		}

		void changeMap(int x,int y)
		{
			map.x = x;
			map.y = y;
		}

		Coordinates currentMap()
		{
			return map;
		}

		void render()
		{
			background_images[map.x][map.y].render(NULL,NULL,true);
		}

		void clear()
		{
			for(int i = 0; i < 8; i++)
			{
				for(int j = 0; j < 11; j++)
				{
					background_images[i][j].free();
				}
			}
		}
};


// Now what is left is buildings, their collisions, professor and collision, ATM and collision
// Batchmates and collision
// Messages to be popped up with meeting people and the start instruciton and music 
// The winning strategy and the timer

// Added the rotation of player while moving

// Add the feature of enter press in order to meet professor, then pop up the screen which is gone
// after some button or mouse press, do this for 1 building then the similar logic can
// be implemented for yulu stand, and football ground. then finally music will be left
// for the background tiles, create another matrix coordinate


/* Final things left to be done 
1. Implement the background class and create a separate load media function to load all the textures
2. Position all the texture and buildings
3. Correct the player's movement 
4. Implement the timer meter
5. Implement the server and the client function
6. Inside the main while loop create different while loop dependig on server or client
7. send coordinates of player in each frame and render accordingly
8. Initialize page of the game*/