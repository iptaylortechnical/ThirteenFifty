all: client

client:
	mkdir -p bin
	$(CC) -g src/*.c -o bin/ntftp
