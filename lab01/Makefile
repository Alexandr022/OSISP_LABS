CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

TARGET = main
SRC = dirwalk.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
