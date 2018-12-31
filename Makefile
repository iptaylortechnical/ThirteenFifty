GCC?=gcc

all: client

client:
	mkdir -p bin
	$(GCC) -g src/*.c -o bin/ntftp