#include "game.h"

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
        while(!quit)
        {
            while(SDL_PollEvent(&event) != 0)
            {
                if( event.type == SDL_QUIT)
                {
                    quit = true;
                }
                light.handleEvent(event);
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
            meter.render(NULL,&meterDisplay,true);
            SDL_RenderPresent(renderer);
            meter.free();
        }
        background.free();
        light.free();
        coin.free();
        destroy();
        return 0;
    }
    
}