#include "game.h"

void print(vector<int> vec)
{
    for(int i =0; i < vec.size();i++)
    {
        cout << vec[i] << " ";
    }
    cout << endl;
}

int main(int argc, char* argv[])
{
    if(!initialize())
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        bool quit = false;
        SDL_Event event;
        Textures background;
        Player light("Photos/light-demo.png");
        background.loadTexture("Photos/background-demo.png");
        SDL_Rect start = {200,200,200,200};
        SDL_Rect meterDisplay = {0,0,1000,25};
        vector<Coordinates> coinList = randomCoins();
        vector<Coordinates> boxes = randomTreasure();
        Textures coin;
        Textures box;
        coin.loadTexture("Photos/coin-demo.png");
        box.loadTexture("Photos/treasure-demo.png");
        Textures building;

        // We are gonna test the building rendering and its collision write now(to be changed later)
        building.loadTexture("Photos/building-demo.png");
        SDL_Rect building_destination = {300,300,BUILDING_WIDTH,BUILDING_HEIGHT}; 
        Coordinates building_coordinate;
        building_coordinate.x = 300;
        building_coordinate.y = 300;
        vector<Coordinates> buildings;
        buildings.push_back(building_coordinate);
        Textures display_text;
        display_text.loadTexture("Photos/text-hostel.png");
        Textures textTexture;
        vector<int> showText;
        SDL_Rect text = {0,SCREEN_HEIGHT - 300,SCREEN_WIDTH,300};
        while(!quit)
        {
            while(SDL_PollEvent(&event) != 0)
            {
                if( event.type == SDL_QUIT)
                {
                    quit = true;
                }
                showText = light.handleEvent(event,buildings);
                if(showText.size() > 0 && showText[0] == 0)
                {
                    textTexture.setTexture(display_text.getTexture());
                }
                if(showText.size() > 0 && showText[0] == 1)
                {
                    textTexture.setTexture(NULL);
                }
            }
            vector<vector<int>> coinsAndBox;
            coinsAndBox = light.move(coinList,boxes);
            coinList  = updateCoins(coinList,coinsAndBox[1]);
            boxes = updateBoxes(boxes,coinsAndBox[2]);
            Textures meter = light.displayMeter();
            SDL_RenderClear(renderer);
            background.render(NULL,NULL,true);
            light.render();
            setCoins(coinList,coin);
            setTreasure(boxes,box);
            building.render(NULL,&building_destination,true);
            meter.render(NULL,&meterDisplay,true);
            textTexture.render(NULL,&text,true);
            SDL_RenderPresent(renderer);
            meter.free();
        }
        textTexture.free();
        display_text.free();
        building.free();
        background.free();
        light.free();
        coin.free();
        destroy();
        return 0;
    }
    
}