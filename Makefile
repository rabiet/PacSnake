CC=gcc
# unpacked SDL2 into a folder named SDL2 and not to C:/SDL2
# SDL2.dll moved from SDL2\x86_64-w64-mingw32\bin to project root
CFLAGS=-I".\SDL2\x86_64-w64-mingw32\include" -std=c99
LDFLAGS=-L".\SDL2\x86_64-w64-mingw32\lib" -lmingw32 -lSDL2main -lSDL2_ttf -lSDL2 -lopengl32

all: run

PacSnake.exe:
	$(CC) $(CFLAGS) -o PacSnake.exe PacSnake.c Ghosts.c Player.c PowerUps.c Map.c Position.c $(LDFLAGS)

build:
	$(CC) $(CFLAGS) -o PacSnake.exe PacSnake.c Ghosts.c Player.c PowerUps.c Map.c Position.c $(LDFLAGS)

run: PacSnake.exe
	PacSnake.exe
