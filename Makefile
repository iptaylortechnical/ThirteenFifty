CC=gcc

all: client

client:
	$(CC) -g src/client.c -o bin/t_fifty