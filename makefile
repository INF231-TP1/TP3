CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = ppmviewer
SOURCES = main.c tp3.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) -lm

clean:
	rm -f $(TARGET) *.o

.PHONY: clean
