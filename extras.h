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
const int COIN_WIDTH = 60;
const int COIN_HEIGHT = 60;
const int TREASURE_WIDTH = 150;
const int TREASURE_HEIGHT = 150;
const int PLAYER_WIDTH = 300;
const int PLAYER_HEIGHT = 150;
const int BUILDING_WIDTH = 700;
const int BUILDING_HEIGHT = 700;
const int YULUSTAND_WIDTH = 300;
const int YULUSTAND_HEIGHT = 300;
const int GROUND_WIDTH = 700;
const int GROUND_HEIGHT = 400;
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
		int mapX;
		int mapY;
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
vector<vector<int>> checkAllCollisions(SDL_Rect player_rect,vector<Coordinates> coinsList,vector<Coordinates> boxes,int mapX,int mapY)
{
	vector<vector<int>> result;
	int coinsCollided = 0;
	int boxesCollided = 0;
	vector<int> coinIndex;
	vector<int> boxIndex;
	
	for(int i =0;i < coinsList.size();i++)
	{
		SDL_Rect coin_rect = {coinsList[i].x,coinsList[i].y,COIN_WIDTH,COIN_HEIGHT};
		if(!(coinsList[i].collided) && (coinsList[i].mapX == mapX) && (coinsList[i].mapY == mapY))
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
		if(!(boxes[i].collided) && (boxes[i].mapX == mapX) && (boxes[i].mapY == mapY))
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
Coordinates building_collision(SDL_Rect player_pos, vector<Coordinates> buildings,int mapX,int mapY)
{
	for(int i =0; i < buildings.size(); i++)
	{
		SDL_Rect building = {buildings[i].x,buildings[i].y,BUILDING_WIDTH,BUILDING_HEIGHT};
		if(checkCollision(player_pos,building) && buildings[i].mapX == mapX && buildings[i].mapY == mapY)
		{
			return buildings[i];
		}
	}
	Coordinates result;
	result.x = -1;
	result.y = -1;
	return result;
}

Coordinates yulu_collision(SDL_Rect player_pos, vector<Coordinates> yuluStand,int mapX,int mapY)
{
	for(int i =0; i < yuluStand.size(); i++)
	{
		SDL_Rect yulu = {yuluStand[i].x,yuluStand[i].y,YULUSTAND_WIDTH,YULUSTAND_HEIGHT};
		if(checkCollision(player_pos,yulu) && yuluStand[i].mapX == mapX && yuluStand[i].mapY == mapY)
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
	int totalBoxes = 50;
	vector<Coordinates> result;
	srand(time(0));
	while (boxes < totalBoxes)
	{
		int x = rand() % (SCREEN_WIDTH - TREASURE_WIDTH);
		int y = rand() % (SCREEN_HEIGHT - TREASURE_HEIGHT);
		int mapX = rand() % 8;
		int mapY = rand() % 11;
		Coordinates coordinate;
		coordinate.x = x;
		coordinate.y = y;
		coordinate.mapX = mapX;
		coordinate.mapY = mapY;
		result.push_back(coordinate);
		boxes++;
	}
	return result;
}

// This function will render the boxes
void setTreasure(vector<Coordinates> boxes, Textures box,int mapX,int mapY)
{
	for(int i =0; i < boxes.size();i++)
	{
		if(!(boxes[i].collided) && (boxes[i].mapX == mapX) && (boxes[i].mapY == mapY))
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
	int totalCoins = 1000;
	vector<Coordinates> result;
	srand(time(0));
	while (numCoins < totalCoins)
	{
		int x = rand() % (SCREEN_WIDTH - COIN_WIDTH);
		int y = rand() % (SCREEN_HEIGHT - COIN_HEIGHT);
		int mapX = rand() % 8;
		int mapY = rand() % 11;
		Coordinates coordinate;
		coordinate.x = x;
		coordinate.y = y;
		coordinate.mapX = mapX;
		coordinate.mapY = mapY;
		result.push_back(coordinate);
		numCoins++;
	}
	return result;
}

// height and width of coins are 20 pixel each also we will use the same function to set treasure
void setCoins(vector<Coordinates> coinsList, Textures coin,int mapX,int mapY)
{
	for(int i =0; i < coinsList.size();i++)
	{
		if(!(coinsList[i].collided) && (coinsList[i].mapX == mapX) && (coinsList[i].mapY == mapY))
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
// This function will give the texture of player meter to be displayed
Textures displayTime(int currentTime)
{
	Textures meter;
	int seconds = currentTime / 1000;
	int minutes = seconds / 60;
	seconds = seconds % 60;
	string meterText = "Time : " + to_string(minutes) + " min " + to_string(seconds) + "seconds";
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

// HostelTextures made global
vector<Textures> HostelTextures;
void createHostels()
{

	// Textures loaded
	Textures aravali;
	Textures karakoram;
	Textures jwala;
	Textures himadri;
	Textures kailash;
	Textures kumaon;
	Textures nilgiri;
	Textures satpura;
	Textures girnar;
	Textures vidhya;
	aravali.loadTexture("Graphics_nobg/Aravali.png");
	karakoram.loadTexture("Graphics_nobg/Karakoram.png");
	jwala.loadTexture("Graphics_nobg/Jwala.png");
	himadri.loadTexture("Graphics_nobg/Himadri.png");
	kailash.loadTexture("Graphics_nobg/Kailash.png");
	kumaon.loadTexture("Graphics_nobg/Kumaon.png");
	nilgiri.loadTexture("Graphics_nobg/Nilgiri.png");
	satpura.loadTexture("Graphics_nobg/Satpura.png");
	girnar.loadTexture("Graphics_nobg/girnar.png");
	vidhya.loadTexture("Graphics_nobg/Vindhyanchal.png");

	// Textures appended
	HostelTextures.push_back(aravali);
	HostelTextures.push_back(karakoram);
	HostelTextures.push_back(jwala);
	HostelTextures.push_back(himadri);
	HostelTextures.push_back(kailash);
	HostelTextures.push_back(kumaon);
	HostelTextures.push_back(nilgiri);
	HostelTextures.push_back(satpura);
	HostelTextures.push_back(girnar);
	HostelTextures.push_back(vidhya);
}

// destroy the hostel textures
void clearHostels()
{
	for (int i = 0; i < HostelTextures.size(); i++)
	{
		HostelTextures[i].free();
	}
}

// This function will give the coordinates of hostel in the map
vector<Coordinates> hostelCoordinates()
{
	// The coordinates of hostels in the map
	Coordinates aravali;
	Coordinates karakoram;
	Coordinates jwala;
	Coordinates himadri;
	Coordinates kailash;
	Coordinates kumaon;
	Coordinates nilgiri;
	Coordinates satpura;
	Coordinates girnar;
	Coordinates vidhya;

	aravali.x = 900;
	aravali.y = 800;
	aravali.mapX = 3;
	aravali.mapY = 0;

	karakoram.x = 900;
	karakoram.y = 100;
	karakoram.mapX = 3;
	karakoram.mapY = 0;

	jwala.x = 900;
	jwala.y = 500;
	jwala.mapX = 2;
	jwala.mapY = 0;

	himadri.x = 900;
	himadri.y = 50;
	himadri.mapX = 2;
	himadri.mapY = 9;

	kailash.x = 50;
	kailash.y = 400;
	kailash.mapX = 2;
	kailash.mapY = 9;

	kumaon.x = 50;
	kumaon.y =  800;
	kumaon.mapX = 2;
	kumaon.mapY = 1;

	nilgiri.x = 900;
	nilgiri.y = 700;
	nilgiri.mapX = 4;
	nilgiri.mapY = 0;

	satpura.x = 800;
	satpura.y = 500;
	satpura.mapX = 2;
	satpura.mapY = 4;

	girnar.x = 50;
	girnar.y = 50;
	girnar.mapX = 3;
	girnar.mapY = 5;

	vidhya.x = 860;
	vidhya.y = 900;
	vidhya.mapX = 2;
	vidhya.mapY = 2;

	vector<Coordinates> hostelPos;
	hostelPos.push_back(aravali);
	hostelPos.push_back(karakoram);
	hostelPos.push_back(jwala);
	hostelPos.push_back(himadri);
	hostelPos.push_back(kailash);
	hostelPos.push_back(kumaon);
	hostelPos.push_back(nilgiri);
	hostelPos.push_back(satpura);
	hostelPos.push_back(girnar);
	hostelPos.push_back(vidhya);
	
	return hostelPos;
}

// This will render the hostel on map
void renderHostel(int mapX,int mapY,vector<Coordinates> hostelPos)
{
	for(int i =0; i < hostelPos.size(); i++)
	{
		if(hostelPos[i].mapX == mapX && hostelPos[i].mapY == mapY)
		{
			SDL_Rect destination = {hostelPos[i].x,hostelPos[i].y,BUILDING_WIDTH,BUILDING_HEIGHT};
			SDL_RenderCopy(renderer,HostelTextures[i].getTexture(),NULL,&destination);
		}
	}
}

vector<Coordinates> yuluCoordinates()
{
	Coordinates yulu1;
	Coordinates yulu2;
	Coordinates yulu3;
	Coordinates yulu4;
	Coordinates yulu5;
	Coordinates yulu6;
	Coordinates yulu7;
	
	yulu1.x = 20;
	yulu1.y = 500;
	yulu1.mapX = 2;
	yulu1.mapY = 8;

	yulu2.x = 1300;
	yulu2.y = 250;
	yulu2.mapX = 4;
	yulu2.mapY = 7;

	yulu3.x = 350;
	yulu3.y = 600;
	yulu3.mapX = 7;
	yulu3.mapY = 4;

	yulu4.x = 950;
	yulu4.y = 750;
	yulu4.mapX = 6;
	yulu4.mapY = 2;

	yulu5.x = 900;
	yulu5.y = 400;
	yulu5.mapX = 3;
	yulu5.mapY = 4;

	yulu6.x = 700;
	yulu6.y = 50;
	yulu6.mapX = 3;
	yulu6.mapY = 9;

	yulu7.x = 1000;
	yulu7.y = 450;
	yulu7.mapX = 5;
	yulu7.mapY = 0;

	vector<Coordinates> yuluStand;
	yuluStand.push_back(yulu1);
	yuluStand.push_back(yulu2);
	yuluStand.push_back(yulu3);
	yuluStand.push_back(yulu4);
	yuluStand.push_back(yulu5);
	yuluStand.push_back(yulu6);
	yuluStand.push_back(yulu7);

	return yuluStand;
}

// This will render yulu on the map
void renderYulu(int mapX,int mapY, Textures yulu, vector<Coordinates> yuluStand)
{
	for(int i =0; i < yuluStand.size(); i++)
	{
		if(yuluStand[i].mapX == mapX && yuluStand[i].mapY == mapY)
		{
			SDL_Rect destination = {yuluStand[i].x,yuluStand[i].y,YULUSTAND_WIDTH,YULUSTAND_HEIGHT};
			SDL_RenderCopy(renderer,yulu.getTexture(),NULL,&destination);
		}
	}	
}


vector<Coordinates> footballCoordinates()
{
	Coordinates ground;
	ground.x = 860;
	ground.y = 650;
	ground.mapX = 5;
	ground.mapY = 4;
	vector<Coordinates> result;
	result.push_back(ground);
	return result;
}

// This will render the football ground on the map
void renderGround(int mapX,int mapY,Textures ground, vector<Coordinates> groundCoordinates)
{
	for(int i =0; i < groundCoordinates.size(); i++)
	{
		if(groundCoordinates[i].mapX == mapX && groundCoordinates[i].mapY == mapY)
		{
			SDL_Rect destination = {groundCoordinates[i].x,groundCoordinates[i].y,GROUND_WIDTH,GROUND_HEIGHT};
			SDL_RenderCopy(renderer,ground.getTexture(),NULL,&destination);
		}
	}	
}

vector<Coordinates> masalaMixCoordinates()
{
	Coordinates masala;
	masala.x = 900;
	masala.y = 400;
	masala.mapX = 4;
	masala.mapY = 3;
	vector<Coordinates> result;
	result.push_back(masala);
	return result;
}

// This will render the masala mix ground on the map
void renderMasala(int mapX,int mapY,Textures ground, vector<Coordinates> groundCoordinates)
{
	for(int i =0; i < groundCoordinates.size(); i++)
	{
		if(groundCoordinates[i].mapX == mapX && groundCoordinates[i].mapY == mapY)
		{
			SDL_Rect destination = {groundCoordinates[i].x,groundCoordinates[i].y,BUILDING_WIDTH,BUILDING_HEIGHT};
			SDL_RenderCopy(renderer,ground.getTexture(),NULL,&destination);
		}
	}	
}

vector<Coordinates> professorCoordinates()
{
	Coordinates prof;
	prof.mapX = 4;
	prof.mapY = 7;
	prof.x = 1300;
	prof.y = 700;

	// Coordinates prof2;

	vector<Coordinates> result;
	result.push_back(prof);
	return result;
}

// This will render the masala mix ground on the map
void renderProf(int mapX,int mapY,Textures ground, vector<Coordinates> groundCoordinates)
{
	for(int i =0; i < groundCoordinates.size(); i++)
	{
		if(groundCoordinates[i].mapX == mapX && groundCoordinates[i].mapY == mapY)
		{
			SDL_Rect destination = {groundCoordinates[i].x,groundCoordinates[i].y,BUILDING_WIDTH,BUILDING_HEIGHT};
			SDL_RenderCopy(renderer,ground.getTexture(),NULL,&destination);
		}
	}	
}

// "10 23 34 45 4 5 200 400 1"
// We will make some extra functions to send and receive data using sockets

string paramToData(vector<int> playerParam)
{
	// 0 -> coins
	// 1 -> health
	// 2 -> stamina
	// 3 -> motivation
	// 4 -> mapX
	// 5 -> mapY
	// 6 -> x
	// 7 -> y
	// 8 -> onYulu
	string value = "" + to_string(playerParam[0]) + " " + to_string(playerParam[1]) + " ";
	value += to_string(playerParam[2]) + " " + to_string(playerParam[3]) + " " + to_string(playerParam[4]);
	value += " " + to_string(playerParam[5]) + " " + to_string(playerParam[6]) + " " + to_string(playerParam[7]);
	value += " " + to_string(playerParam[8]);
	string result = value.c_str();
	return result;
}

vector<int> dataToParam(string value)
{
	vector<int> result;
	if(value.compare("") == 0)
	{
		return result;
	}
	else
	{
		int index = 0;
		int prev = 0;
		while(index < value.size())
		{
			if((value.substr(index,1)).compare(" ") == 0)
			{
				int num = stoi(value.substr(prev,index - prev));
				result.push_back(num);
				index ++;
				prev = index;
			}
			else
			{
				index ++;
			}

		}
        int num = stoi(value.substr(prev,index - prev));
        result.push_back(num);
		return result;
	}
}

