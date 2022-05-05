#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <time.h>
#include <vector>

using namespace std;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* meterFont = NULL;
Mix_Music* music1 = NULL;
Mix_Chunk* coinCollect = NULL;
Mix_Chunk* treasureCollect = NULL;
Mix_Chunk* yuluChange = NULL;
Mix_Chunk* thanks = NULL;

const int SCREEN_WIDTH = 1720;
const int SCREEN_HEIGHT = 1000;
const int COIN_WIDTH = 30;
const int COIN_HEIGHT = 30;
const int TREASURE_WIDTH = 50;
const int TREASURE_HEIGHT = 50;
const int PLAYER_WIDTH = 200;
const int PLAYER_HEIGHT = 150;
const int BUILDING_WIDTH = 200;
const int BUILDING_HEIGHT = 200;
const int YULUSTAND_WIDTH = 200;
const int YULUSTAND_HEIGHT = 200;

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

void loadAudio()
{
	coinCollect = Mix_LoadWAV("Audio/coin.wav");
	if( coinCollect == NULL )
    {
        printf( "Failed to load coin collect music! SDL_mixer Error: %s\n", Mix_GetError() );
    }
	treasureCollect = Mix_LoadWAV("Audio/treasure.wav");
	if( treasureCollect == NULL )
    {
        printf( "Failed to load treasure collect music! SDL_mixer Error: %s\n", Mix_GetError() );
    }
	yuluChange = Mix_LoadWAV("Audio/yulu-change.wav");
	if( yuluChange == NULL )
    {
        printf( "Failed to load yulu music! SDL_mixer Error: %s\n", Mix_GetError() );
    }
	thanks = Mix_LoadWAV("Audio/thanks.wav");
	if( thanks == NULL )
    {
        printf( "Failed to load thanks music! SDL_mixer Error: %s\n", Mix_GetError() );
    }
	music1 = Mix_LoadMUS("Audio/music1.wav");
	if( music1 == NULL )
    {
        printf( "Failed to load first music! SDL_mixer Error: %s\n", Mix_GetError() );
    }
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

// Now we will implement a function to produce random gift from the treasure box
vector<int> gift()
{
	srand(time(0));
	int motivation = (rand() % 100) - 50;
	int stamina = (rand() % 100) - 50;
	int health = (rand() % 100) - 50;
	int coins = (rand() % 100) - 50;
	vector<int> result;
	result.push_back(motivation);
	result.push_back(stamina);
	result.push_back(health);
	result.push_back(coins);
	return result;
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

		void setTexture(SDL_Texture* text)
		{
			texture = text;
		}

		void setDimension(int w,int h)
		{
			width = w;
			height = h;
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
		void render(SDL_Rect* start, SDL_Rect* destination, bool full, double angle = 0.0)
		{
			if(!full)
			{
				SDL_RenderCopyEx(renderer,texture,start,destination,angle,NULL,SDL_FLIP_NONE);
			}
			else
			{
				SDL_RenderCopyEx(renderer,texture,NULL,destination,angle,NULL,SDL_FLIP_NONE);
			}
		}

		SDL_Texture* getTexture()
		{
			return texture;
		}
};

// Coordinates class
class Coordinates
{
	public:
		int x,y;
		bool collided = false;
};

// This function will give collision between 2 rectangles
bool checkCollision( SDL_Rect a, SDL_Rect b )
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;
	//If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

// This function will give all the coins and boxes that are collided with the player
// The index 0 if the vector which contains 2 elements - coinsCollided and boxesCollided
// The index 1 will give the index vector of coinsCollided
// The index 2 will give the index vector of boxesCollided
vector<vector<int>> checkAllCollisions(SDL_Rect player_rect,vector<Coordinates> coinsList,vector<Coordinates> boxes)
{
	vector<vector<int>> result;
	int coinsCollided = 0;
	int boxesCollided = 0;
	vector<int> coinIndex;
	vector<int> boxIndex;
	
	for(int i =0;i < coinsList.size();i++)
	{
		SDL_Rect coin_rect = {coinsList[i].x,coinsList[i].y,COIN_WIDTH,COIN_HEIGHT};
		if(!(coinsList[i].collided))
		{
			if(checkCollision(player_rect,coin_rect))
			{
				coinsCollided ++;
				coinIndex.push_back(i);
				Mix_PlayChannel( -1, coinCollect, 0 );
			}
		}
	}

	for(int i =0;i < boxes.size();i++)
	{
		SDL_Rect box_rect = {boxes[i].x,boxes[i].y,TREASURE_WIDTH,TREASURE_HEIGHT};
		if(!(boxes[i].collided))
		{
			if(checkCollision(player_rect,box_rect))
			{
				boxesCollided++;
				boxIndex.push_back(i);
				Mix_PlayChannel( -1, treasureCollect, 0 );
			}
		}
	}
	vector<int> collidedCoinsBox;
	collidedCoinsBox.push_back(coinsCollided);
	collidedCoinsBox.push_back(boxesCollided);
	result.push_back(collidedCoinsBox);
	result.push_back(coinIndex);
	result.push_back(boxIndex);
	return result;
}


// This function will give the building with which player is colliding
Coordinates building_collision(SDL_Rect player_pos, vector<Coordinates> buildings)
{
	for(int i =0; i < buildings.size(); i++)
	{
		SDL_Rect building = {buildings[i].x,buildings[i].y,BUILDING_WIDTH,BUILDING_HEIGHT};
		if(checkCollision(player_pos,building))
		{
			return buildings[i];
		}
	}
	Coordinates result;
	result.x = -1;
	result.y = -1;
	return result;
}

Coordinates yulu_collision(SDL_Rect player_pos, vector<Coordinates> yuluStand)
{
	for(int i =0; i < yuluStand.size(); i++)
	{
		SDL_Rect yulu = {yuluStand[i].x,yuluStand[i].y,YULUSTAND_WIDTH,YULUSTAND_HEIGHT};
		if(checkCollision(player_pos,yulu))
		{
			return yuluStand[i];
		}
	}
	Coordinates result;
	result.x = -1;
	result.y = -1;
	return result;	
}
// This function will update the coins collided value to true
vector<Coordinates> updateCoins(vector<Coordinates> coinsList,vector<int> coinIndex)
{
	for(int i =0; i < coinIndex.size();i++)
	{
		coinsList[coinIndex[i]].collided = true;
	}
	return coinsList;
}

vector<Coordinates> updateBoxes(vector<Coordinates> boxes, vector<int> boxIndex)
{
	for(int i =0; i < boxIndex.size();i++)
	{
		boxes[boxIndex[i]].collided = true;
	}
	return boxes;
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


// This function will be called to give the location of different coins randomly 
vector<Coordinates> randomCoins()
{
	int numCoins = 0;
	int totalCoins = 50;
	vector<Coordinates> result;
	srand(time(0));
	while (numCoins < totalCoins)
	{
		int x = rand() % (SCREEN_WIDTH - COIN_WIDTH);
		int y = rand() % (SCREEN_HEIGHT - COIN_HEIGHT);
		Coordinates coordinate;
		coordinate.x = x;
		coordinate.y = y;
		result.push_back(coordinate);
		numCoins++;
	}
	return result;
}

// height and width of coins are 20 pixel each also we will use the same function to set treasure
void setCoins(vector<Coordinates> coinsList, Textures coin)
{
	for(int i =0; i < coinsList.size();i++)
	{
		if(!(coinsList[i].collided))
		{
			SDL_Rect destination;
			destination.x = coinsList[i].x;
			destination.y = coinsList[i].y;
			destination.w = COIN_WIDTH;
			destination.h = COIN_HEIGHT;
			coin.render(NULL,&destination,true);
		}
	}
}

//Now we will generate treasure boxes randomly between < 10
vector<Coordinates> randomTreasure()
{
	srand(time(0));
	int boxes = 0;
	int totalBoxes = rand() % 10;
	vector<Coordinates> result;
	srand(time(0));
	while (boxes < totalBoxes)
	{
		int x = rand() % (SCREEN_WIDTH - TREASURE_WIDTH);
		int y = rand() % (SCREEN_HEIGHT - TREASURE_HEIGHT);
		Coordinates coordinate;
		coordinate.x = x;
		coordinate.y = y;
		result.push_back(coordinate);
		boxes++;
	}
	return result;
}

// This function will render the boxes
void setTreasure(vector<Coordinates> boxes, Textures box)
{
	for(int i =0; i < boxes.size();i++)
	{
		if(!(boxes[i].collided))
		{
			SDL_Rect destination;
			destination.x = boxes[i].x;
			destination.y = boxes[i].y;
			destination.w = TREASURE_WIDTH;
			destination.h = TREASURE_HEIGHT;
			box.render(NULL,&destination,true);
		}
	}
}

void musicPlayer(SDL_Event event)
{
	if(event.type == SDL_KEYDOWN)
	{
		switch( event.key.keysym.sym )
		{
			case SDLK_m:
				//If there is no music playing
				if( Mix_PlayingMusic() == 0 )
				{
					//Play the music
					Mix_PlayMusic( music1, -1 );
				}
				//If music is being played
				else
				{
					//If the music is paused
					if( Mix_PausedMusic() == 1 )
					{
						//Resume the music
						Mix_ResumeMusic();
					}
					//If the music is playing
					else
					{
						//Pause the music
						Mix_PauseMusic();
					}
				}
				break;
				
				case SDLK_s:
				//Stop the music
				Mix_HaltMusic();
				break;
		}
	}
}


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