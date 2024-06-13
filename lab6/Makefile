CC=gcc
CFLAGS=-pthread -g -ggdb -pedantic -std=c11 -W -Wall -Wextra
BIN_DIR=bin

all: main generator reader

main: main.c
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $^

generator: generator.c
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $^

reader: reader.c
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $^

clean:
	rm -rf $(BIN_DIR)
