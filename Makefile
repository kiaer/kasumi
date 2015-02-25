TARGET = bin/kasumi
LIBS = -lm
CC = gcc
CFLAGS = -Ofast -g -Wall

.PHONY: default all clean

default: clean $(TARGET)
all: default
cipher: clean $(TARGET)

OBJECTS = $(patsubst %.c, %.o, $(wildcard src/cipher/*.c))
HEADERS = $(wildcard src/cipher/*.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f src/cipher/*.o
	-rm -f $(TARGET)
