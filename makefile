all:
	g++ demoSdl.cpp -o testing -lSDL2
	g++ justdemo.cpp -o testing2 -lSDL2 -lSDL2_image
	g++ mygame.cpp -o game -lSDL2 -lSDL2_image
remove:
	rm -r testing
	rm -r testing2
	rm -r game