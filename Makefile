CC=gcc

all: client

client:
	mkdir -p bin
	$(CC) -g src/client.c -o bin/t_fifty