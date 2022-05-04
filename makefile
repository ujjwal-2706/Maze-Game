all:
	g++ game.cpp -o game -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
clean:
	rm -r game