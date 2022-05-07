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
		bool onLeft = false;
	public:

		// This will make the player as L or Light (Game characters) and assign it random position
		Player(string path, string yuluPath)
		{
			map.x = 2;
			map.y = 8;
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

		// This function will send the statistics of 1 player to another
		vector<int> statistics()
		{
			vector<int> result;
			result.push_back(coins);
			result.push_back(health);
			result.push_back(stamina);
			result.push_back(motivation);
			result.push_back(map.x);
			result.push_back(map.y);
			result.push_back(x);
			result.push_back(y);
			int yu = 0;
			if(onYulu)
			{
				yu = 1;
				result.push_back(yu);
				return result;
			}
			else
			{
				yu = 0;
				result.push_back(yu);
				return result;
			}
		}

		void renderOther(int xco,int yco,int mapX,int mapY,int yul)
		{
			map.x = mapX;
			map.y = mapY;
			x = xco;
			y = yco;
			if(yul == 0)
			{
				onYulu = false;
			}
			else if(yul == 1)
			{
				onYulu = true;
			}
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
			SDL_Color textColor = {0,128,255};
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
				if(!onLeft)
				{
					SDL_RenderCopyEx(renderer,player_image.getTexture(),NULL,&destination,angle,NULL,SDL_FLIP_NONE);
				}
				else
				{
					SDL_RenderCopyEx(renderer,player_image.getTexture(),NULL,&destination,angle,NULL,SDL_FLIP_HORIZONTAL);
				}
			}
			else
			{
				SDL_Rect destination = {x,y,PLAYER_WIDTH,PLAYER_HEIGHT};
				if(!onLeft)
				{
					SDL_RenderCopyEx(renderer,playerYulu_image.getTexture(),NULL,&destination,angle,NULL,SDL_FLIP_NONE);
				}
				else
				{
					SDL_RenderCopyEx(renderer,playerYulu_image.getTexture(),NULL,&destination,angle,NULL,SDL_FLIP_HORIZONTAL);
				}
				
			}
			
		}

		// Handling the movement of player by changing its velocity 
		void handleEvent(SDL_Event event, vector<Coordinates> buildings,vector<Coordinates> yuluStand,vector<Coordinates> masalaMix,vector<Coordinates> football,vector<Coordinates> professor)
		{
			SDL_Rect player_pos = {x,y,PLAYER_WIDTH,PLAYER_HEIGHT};
			// If a key is pressed and no collision with building 
			if(event.type == SDL_KEYDOWN && event.key.repeat == 0)
			{
				switch( event.key.keysym.sym)
				{
					case SDLK_UP: velY -= 7; angle += 90; break;
					case SDLK_DOWN: velY += 7; angle -= 90; break;
					case SDLK_LEFT: velX -= 7; onLeft = true; break;
					case SDLK_RIGHT: velX += 7; onLeft = false; break;
					case SDLK_0:
						if(building_collision(player_pos,buildings,map.x,map.y).x != -1)
						{
							Mix_PlayChannel(-1,thanks,0);
							coins -= 10;
							health  += 50;
						}

						if(yulu_collision(player_pos,yuluStand,map.x,map.y).x != -1)
						{
							if(!onYulu)
							{
								Mix_PlayChannel(-1,yuluChange,0);
							}
							onYulu = true;
						}

						if(building_collision(player_pos,masalaMix,map.x,map.y).x != -1)
						{
							Mix_PlayChannel(-1,thanks,0);
							coins -= 20;
							health += 80;
							motivation -= 5;
						}
						if(building_collision(player_pos,football,map.x,map.y).x != -1)
						{
							Mix_PlayChannel(-1,thanks,0);
							health += 100;
							motivation += 20;
							stamina -= 20;
						}
						if(building_collision(player_pos,professor,map.x,map.y).x != -1)
						{
							Mix_PlayChannel(-1,thanks,0);
							motivation += 1;
						}
						break;
					case SDLK_1:
						if(yulu_collision(player_pos,yuluStand,map.x,map.y).x != -1)
						{
							if(onYulu)
							{
								Mix_PlayChannel(-1,yuluChange,0);
							}
							onYulu = false;	
						}
						break;

				}
			}

			//If a key was released
			else if( event.type == SDL_KEYUP && event.key.repeat == 0 )
			{
				//Adjust the velocity
				switch( event.key.keysym.sym )
				{
					case SDLK_UP: velY += 7; angle -= 90;  break;
					case SDLK_DOWN: velY -= 7; angle += 90; break;
					case SDLK_LEFT: velX += 7; break;
					case SDLK_RIGHT: velX -= 7; break;
				}
			}
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

			if( ( y + PLAYER_HEIGHT > SCREEN_HEIGHT ) && map.x < 7 )
			{
				y = 0;
				map.x +=1;
			}

			if( ( y + PLAYER_HEIGHT > SCREEN_HEIGHT )  && map.x == 7)
			{
				y -= 2 * velY;
			}


			SDL_Rect player_rect = {x,y,PLAYER_WIDTH,PLAYER_HEIGHT};
			vector<vector<int>> meterIncrease;
			meterIncrease =  checkAllCollisions(player_rect,coinList,boxes,map.x,map.y);
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
			map.x = 2;
			map.y = 8;
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

double pointScored(int coins,int health,int stamina,int motivation)
{
	double value = 2 * coins + stamina + health + 0.01 * motivation;
	return value;
}

Textures displayPoints(double points)
{
	string val =  "Total Points : " + to_string(points);
	Textures meter;
	//Open the font
	meterFont = TTF_OpenFont( "Fonts/beleq.ttf", 28 );
	SDL_Color textColor = {204,0,102};
	if( meterFont == NULL )
	{
		printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
		return meter;
	}
	else
	{
		//Render text surface
		SDL_Surface* textSurface = TTF_RenderText_Solid( meterFont, val.c_str(), textColor );
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