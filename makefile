all:
	g++ game.cpp -o game -lSDL2 -lSDL2_image
clean:
	rm -r game