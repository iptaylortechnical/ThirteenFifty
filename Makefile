GCC?=gcc

all: client

client:
	mkdir -p bin
	$(GCC) -g src/client.c -o bin/t_fifty