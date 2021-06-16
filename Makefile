all: build clean

build: minimax.c
	@gcc -g -Wall -o minimax minimax.c

clean:
	@rm minimax
