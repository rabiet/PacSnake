CC=gcc
# unpacked SDL2 into a folder named SDL2 and not to C:/SDL2
# SDL2.dll moved from SDL2\x86_64-w64-mingw32\bin to project root
CFLAGS=-I".\SDL2\x86_64-w64-mingw32\include"
LDFLAGS=-L".\SDL2\x86_64-w64-mingw32\lib" -lmingw32 -lSDL2main -lSDL2 -lopengl32

all: run

testSDL.exe:
	$(CC) $(CFLAGS) -o testSDL.exe testSDL.c $(LDFLAGS)

run: testSDL.exe
	testSDL.exe
