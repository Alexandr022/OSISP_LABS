CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11

TARGET = main
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
