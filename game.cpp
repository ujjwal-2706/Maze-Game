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
            SDL_RenderClear(renderer);
            background.render(NULL,NULL,true);
            light.render();
            SDL_RenderPresent(renderer);
        }
        background.free();
        light.free();
        destroy();
        return 0;
    }
    
}