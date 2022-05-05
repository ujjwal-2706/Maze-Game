#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <time.h>

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

// Coordinates class (contains coordinates of point)
class Coordinates
{
	public:
		int x,y;
		bool collided = false;
};


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
