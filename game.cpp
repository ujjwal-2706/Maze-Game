#include "game.h"

int main(int argc, char* argv[])
{
    if(!initialize())
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        // load the audio of game
        loadAudio();

        // load hostels and its coordinates
        createHostels();
        vector<Coordinates> hostelPos = hostelCoordinates();

        // load the ground coordinates and its texture
        Textures ground;
        ground.loadTexture("Graphics_nobg/football.png");
        vector<Coordinates> football = footballCoordinates();

        // load the masalaMix coordinates and its texture
        Textures masalaMix;
        masalaMix.loadTexture("Graphics_nobg/Restaurant.png");
        vector<Coordinates> masala = masalaMixCoordinates();

        // load the yulu coordinates and its texture       
        Textures yulu;
        yulu.loadTexture("Graphics_nobg/yulustand.png");
        vector<Coordinates> stand = yuluCoordinates();

         // load the professor coordinates and its texture       
        Textures prof;
        prof.loadTexture("Graphics_nobg/prof.png");
        vector<Coordinates> professor = professorCoordinates();


        // game event handling and quit variables
        bool quit = false;
        SDL_Event event;

        // background loading
        BackGround background;

        // Player loading, coin loading, box loading and meter rectangle
        Player player1("Graphics_nobg/p1.png", "Graphics_nobg/p1c.png");
        SDL_Rect meterDisplay = {0,0,1000,25};
        SDL_Rect timerDisplay = {0,950,1000,50};
        vector<Coordinates> coinList = randomCoins();
        vector<Coordinates> boxes = randomTreasure();
        Textures coin;
        Textures box;
        coin.loadTexture("Graphics_nobg/coin.png");
        box.loadTexture("Graphics_nobg/treasure.png");

        // Start the music
        Mix_PlayMusic( music1, -1);
        while(!quit)
        {
            while(SDL_PollEvent(&event) != 0)
            {
                if( event.type == SDL_QUIT)
                {
                    quit = true;
                }
                musicPlayer(event);
                player1.handleEvent(event,hostelPos,stand,masala,football,professor);
                // if(showText.size() > 0 && showText[0] == 0)
                // {
                //     textTexture.setTexture(display_text.getTexture());
                // }
                // if(showText.size() > 0 && showText[0] == 1)
                // {
                //     textTexture.setTexture(NULL);
                // }
            }

            //Timer
            Textures timer = displayTime(SDL_GetTicks());

            vector<vector<int>> coinsAndBox;
            coinsAndBox = player1.move(coinList,boxes);
            coinList  = updateCoins(coinList,coinsAndBox[1]);
            boxes = updateBoxes(boxes,coinsAndBox[2]);
            Textures meter = player1.displayMeter();

            background.changeMap(player1.getMap().x, player1.getMap().y);
            SDL_RenderClear(renderer);
            background.render();
            renderHostel(player1.getMap().x, player1.getMap().y, hostelPos);
            renderMasala(player1.getMap().x, player1.getMap().y,masalaMix,masala);
            renderGround(player1.getMap().x, player1.getMap().y,ground,football);
            renderYulu(player1.getMap().x, player1.getMap().y,yulu,stand);
            renderProf(player1.getMap().x, player1.getMap().y,prof,professor);
            player1.render();
            setCoins(coinList,coin,player1.getMap().x, player1.getMap().y);
            setTreasure(boxes,box,player1.getMap().x, player1.getMap().y);
            meter.render(NULL,&meterDisplay,true);

            timer.render(NULL,&timerDisplay,true);
            SDL_RenderPresent(renderer);
            meter.free();
            timer.free();
        }

        prof.free();
        masalaMix.free();
        background.clear();
        ground.free();
        yulu.free();
        player1.free();
        coin.free();
        clearHostels();
        destroy();
        return 0;
    }
    
}