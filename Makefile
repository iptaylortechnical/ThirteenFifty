all: client

client:
	mkdir -p bin
	$(CC) src/*.c -o bin/ntftp

clean:
	rm -rf bin
