TARGET = bin/KasumiTable
LIBS = -lm
CC = gcc
CFLAGS = -Ofast -g -Wall

.PHONY: default all clean

default: clean $(TARGET)
all: default
cipher: clean $(TARGET)

OBJECTS = $(patsubst %.c, %.o, $(wildcard src/*.c))
HEADERS = $(wildcard src/*.h)

%.o: %.c $(HEADERS)
	@$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	@mkdir -p bin
	@$(CC) $(OBJECTS) -pg -Wall $(LIBS) -o $@

clean:
	@-rm -f src/cipher/*.o
	@-rm -f $(TARGET)
