# ReadMe For Maze Game

------

For the creation of this game, we have used the following c++ libraries in order to build this game and run it smoothly

- SDL2 library 
- SDL image library 
- SDL mixer library 
- SDL ttf library

NOTE:- Me and my partner are both using Ubuntu 20.04 as an Operating system to play this game

------

### Installation Instructions for the above mentioned SDL libraries:

-  In order to install SDL2 library run the following command

  ```
  sudo apt-get install libsdl2-dev
  ```

- In order to install SDL2 image library run the following command

  ```
  sudo apt-get install libsdl2-image-dev
  ```

- In order to install SDL2 mixer library run the following command

  ```
  sudo apt-get install libsdl2-mixer-dev
  ```

- In order to install SDL2 ttf library run the following command

  ```
  sudo apt-get install libsdl2-ttf-dev
  ```

------

### How to Run the Game:

1. First of all clone this repository on your machine using the following command:-

   ```
   git clone https://github.com/ujjwal-2706/Maze-Game.git
   ```

   or you can simply download the zip folder and extract it on your computer.

2.  Change the current directory to move to this folder in the terminal.

3. Make sure you have installed SDL using the above instructions, you must be using a Linux OS.

4. Now in order to play the game you have 2 choices, either play as a **server** or play as a **client**

   - In order to play as server run the following commands to start the game :-

     ```
     make
     ./game server
     ```

   - In order to play as the client run the following commands to start the game:-

     ```
     make
     ./game client
     ```

5.  It should be noted that the server should start the game first before the client so that connection can be established

6. For further instruction  and controls of the game, please refer to the Presentation.

6.  It should be noted that in order to run the game on 2 computer you will have to hardcode the ip address of the server computer into the client.h file of the client inside the makeClientSide() function.

------

### Online Resources Used:

1.  For studying and understanding the functionalities of SDL library we have referred to [Lazyfoo Website](https://lazyfoo.net/tutorials/SDL/index.php).
2.  For understanding the socket programming in c++ we used the reference from [This website](https://beej.us/guide/bgnet/html/) and [This youtube tutorial](https://www.youtube.com/watch?v=x_y8FDMj6nk&t=1s).
3.  For making the map of IIT Delhi we used the following websites [Pixlr](https://pixlr.com/x/), [Map Editor](https://www.mapeditor.org/) and [Pine Tools](https://pinetools.com/split-image).