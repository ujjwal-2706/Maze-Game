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
        int frames = 0;
        SDL_Rect start = {200,200,200,200};
        SDL_Rect meterDisplay = {0,0,1000,25};
        vector<Coordinates> coinList = randomCoins();
        Textures coin;
        coin.loadTexture("Photos/coin-demo.png");
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
            frames ++;
            cout << frames << endl;
            light.move();
            Textures meter = light.displayMeter();
            SDL_RenderClear(renderer);
            background.render(NULL,NULL,true);
            light.render();
            setCoins(coinList,coin);
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