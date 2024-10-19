CC = gcc
PKG_CONFIG = pkg-config --cflags --libs gtk4
CFLAGS = $(shell $(PKG_CONFIG)) -lm
TARGET = bin/main
SRC = src/main.c src/parser.c src/calculator.c

all: $(TARGET)

bin:
	mkdir -p bin

$(TARGET): bin $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -rf bin

.PHONY: all clean bin

