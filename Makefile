all: client

client:
	mkdir -p bin
	$(CC) src/*.c -o bin/ntftp

clean:
	rm -rf bin

install:
	cp ./bin/ntftp $(TEMP_RFS)/bin
	