all:
	g++ -Iinclude -Iinclude/SDL2 -Iinclude/headers -Llib -o Main src/*.cpp -lmingw32 -lSDL2 -lSDL2_image -lSDL2_ttf -mwindows

debug:
	g++ -Iinclude -Iinclude/SDL2 -Iinclude/headers -Llib -o Main src/*.cpp -lmingw32 -lSDL2 -lSDL2_image -lSDL2_ttf