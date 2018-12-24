CC=gcc

all: client

client:
	$(CC) src/client.c -o bin/t_fifty