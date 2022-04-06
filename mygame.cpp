#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
using namespace std;

SDL_Window* window = NULL;
SDL_Renderer* render = NULL;
SDL_Surface* surface1 = NULL;
SDL_Rect destination;
SDL_Texture* text;
SDL_Texture* text2;
int main(int argc,char* args[])
{
    destination.x = 0;
    destination.y = 0;
    destination.w = 64;
    destination.h = 64;
    int xpos =  0;
    int ypos = 0;
    bool quit = false;
    SDL_Event event;
    window = SDL_CreateWindow("Zero's Game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1720,1080,SDL_WINDOW_SHOWN);
    render = SDL_CreateRenderer(window,-1,0);
    surface1 = SDL_LoadBMP("hello_world.bmp");
    text = SDL_CreateTextureFromSurface(render,surface1);
    SDL_FreeSurface(surface1);
    surface1 = IMG_Load("code.png");
    text2 = SDL_CreateTextureFromSurface(render,surface1);
    SDL_FreeSurface(surface1);
    cout << SDL_GetTicks() << endl;
    SDL_RenderCopy(render,text2,NULL,NULL);
    SDL_RenderCopy(render,text,NULL,&destination);
    SDL_RenderPresent(render);
    while (!quit)
    {

        while(SDL_PollEvent(&event) != 0){
            if(event.type == SDL_QUIT)
            {
                quit = true;
            }

            switch (event.type)
            {
                case SDL_KEYDOWN:
                    if(event.key.keysym.sym == SDLK_a)
                    {
                        if(xpos > 10)
                        {
                            xpos -= 10;
                        }
                    }
                    if(event.key.keysym.sym == SDLK_d)
                    {
                        xpos += 10;
                    }
                    if(event.key.keysym.sym == SDLK_s )
                    {
                        ypos += 10;
                    }
                    if(event.key.keysym.sym == SDLK_w)
                    {
                        ypos -= 10;
                    }
                    break;
                case SDL_KEYUP:

                    break;
                default:
                    break;
            }
        }
        destination.x = xpos;
        destination.y = ypos;
        SDL_RenderClear(render);
        SDL_RenderCopy(render,text2,NULL,NULL);
        SDL_RenderCopy(render,text,NULL,&destination);
        SDL_RenderPresent(render);
    }
    SDL_DestroyRenderer(render);
    SDL_DestroyTexture(text);
    SDL_DestroyWindow(window);
    SDL_Quit();
    cout << "ending game" << endl;
    return 0;
}