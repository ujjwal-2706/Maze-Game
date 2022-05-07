#include "game.h"
#include "server.h"
#include "client.h"

int main(int argc, char** argv)
{
    
    bool server = false;
    bool client = false;
    string side = argv[1];

    if(side.compare("server") == 0)
    {
        server = true;
        makeServerSide();
    }
    if(side.compare("client") == 0)
    {
        client = true;
        makeClientSide();
    }

    if(!initialize())
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        


        // load the audio of game
        loadAudio();

        // load the win, lose, start, instruction page
        Textures win;
        Textures lose;
        Textures rules;
        Textures start;
        win.loadTexture("Graphics_nobg/win.png");
        lose.loadTexture("Graphics_nobg/lose.png");
        rules.loadTexture("Graphics_nobg/Rules.jpg");
        start.loadTexture("Graphics_nobg/Start.jpg");
        bool playerWon = false;
        bool startScreen = true;
        bool instructionScreen = true;
        int timeVal = 0;

        // load hostels and its coordinates
        createHostels();
        vector<Coordinates> hostelPos = hostelCoordinates();

        // load the football ground coordinates and its texture
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
        Player player2("Graphics_nobg/p2.png","Graphics_nobg/p2c.png");
        SDL_Rect meterDisplay = {0,0,1000,25};
        SDL_Rect timerDisplay = {0,950,1000,50};
        vector<Coordinates> coinList = randomCoins();
        vector<Coordinates> boxes = randomTreasure();
        Textures coin;
        Textures box;
        coin.loadTexture("Graphics_nobg/coin.png");
        box.loadTexture("Graphics_nobg/treasure.png");
        vector<int> secondPlayer;
        SDL_Rect pointLocation = {500,900,600,80};

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

                if(event.type == SDL_KEYDOWN && event.key.repeat == 0)
                {
                    switch( event.key.keysym.sym)
                    {
                        case SDLK_n: startScreen = false; break;
                        case SDLK_a:
                            instructionScreen = false;
                            if( Mix_PlayingMusic() == 0 )
                            {
                                //Play the music
                                Mix_PlayMusic( music1, -1 );
                            }
                            break;
                    }
                }
                
            }

            if(server)
            {
                string param = readFromClient();
                secondPlayer = dataToParam(param);
                string messagetoClient = paramToData(player1.statistics());
                const char* mess = messagetoClient.c_str();
                sendToClient(mess);
            }
            else
            {
                string messagetoServer = paramToData(player1.statistics());
                const char* mess = messagetoServer.c_str();
                sendToServer(mess);
                string param = readFromServer();
                secondPlayer = dataToParam(param);
            }

            player2.renderOther(secondPlayer[6],secondPlayer[7],secondPlayer[4],secondPlayer[5],secondPlayer[8]);
            bool renderP2 = false;
            //Timer
            if((!startScreen) && (!instructionScreen) && (SDL_GetTicks() - timeVal < 300000))
            {
                Textures timer = displayTime(SDL_GetTicks() - timeVal);
                vector<vector<int>> coinsAndBox;
                coinsAndBox = player1.move(coinList,boxes);
                if(player1.getMap().x == player2.getMap().x && player1.getMap().y == player2.getMap().y)
                {
                    renderP2 = true;
                }
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
                if(renderP2)
                {
                    player2.render();
                }
                setCoins(coinList,coin,player1.getMap().x, player1.getMap().y);
                setTreasure(boxes,box,player1.getMap().x, player1.getMap().y);
                meter.render(NULL,&meterDisplay,true);
                timer.render(NULL,&timerDisplay,true);
                SDL_RenderPresent(renderer);
                meter.free();
                timer.free();
            }
            else if(SDL_GetTicks() - timeVal >= 300000)
            {
                Mix_HaltMusic();
                vector<int> player1Stat = player1.statistics();
                double p1 = pointScored(player1Stat[0],player1Stat[1],player1Stat[2],player1Stat[3]);
                double p2 = pointScored(secondPlayer[0],secondPlayer[1],secondPlayer[2],secondPlayer[3]);
                Textures pointMeter = displayPoints(p1);
                SDL_RenderClear(renderer);
                if(p1 >= p2)
                {
                    win.render(NULL,NULL,true);
                }
                else
                {
                    lose.render(NULL,NULL,true);
                }
                pointMeter.render(NULL,&pointLocation,true);
                SDL_RenderPresent(renderer);
                pointMeter.free();
            }
            else
            {
                timeVal = SDL_GetTicks();
                SDL_RenderClear(renderer);
                 if(instructionScreen)
                {
                    rules.render(NULL,NULL,true);
                }
                if(startScreen)
                {
                    start.render(NULL,NULL,true);
                }
                SDL_RenderPresent(renderer);
            }
        }

        start.free();
        rules.free();
        win.free();
        lose.free();
        prof.free();
        masalaMix.free();
        background.clear();
        ground.free();
        yulu.free();
        player1.free();
        player2.free();
        coin.free();
        clearHostels();
        destroy();
        return 0;
    }
    
}
