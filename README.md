# PacSnake

PacSnake is a mixture of two games: PacMan and Snake. 
In this game you play as a snake in the PacMan field. Your ultimate goal is growing bigger by eating food. 
There is also the possibility to eat power-ups that might either help you or not.

# Coding

This project was coded using C and the SDL2 libraries.

## Build instructions
You can compile PacSnake using the provided makefile:
* Windows
  * `mingw32-make build`
* Linux
  * `make build-linux`


**On both systems it is required to have both SDL2 and SDL2-TTF installed and properly configured!
When actually playing the game, make sure you take all files from the dependency folder and put it in the folder with the actual executable, otherwise the game will not start!**
