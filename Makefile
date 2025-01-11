all:
	g++ -Iinclude -Iinclude/SDL2 -Iinclude/headers -Llib -o pong src/*.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -mwindows

debug:
	g++ -Iinclude -Iinclude/SDL2 -Iinclude/headers -Llib -o pong src/*.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf