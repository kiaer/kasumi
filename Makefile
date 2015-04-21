TARGET = bin/KasumiTable
TARGETC = bin/kasumi_test
LIBS = -lm
CC = gcc
CFLAGS = -Ofast -g -Wall

.PHONY: default all clean

default: clean $(TARGET)
all: default
cipher: cleanciph $(TARGETC)

OBJECTS = $(patsubst %.c, %.o, $(wildcard src/*.c))
HEADERS = $(wildcard src/*.h)
#OBJECTSCIPH = $(wildcard src/cipher/*.c)
#HEADERSCIPH = $(wildcard src/cipher/*.h)

%.o: %.c $(HEADERS)
	@$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	@echo "Compiling RainbowTable generator.."
	@mkdir -p bin
	@$(CC) $(OBJECTS) $(LIBS) -o $@
	@echo "Tablegenerator compiled with" $(CFLAGS)

$(TARGETC): $(OBJECTSCIPH)
	@echo "Compiling cipher.."
	@mkdir -p bin
	@$(CC) src/cipher/kasumi_test.c $(CFLAGS) -pg -o $@
	@echo "Cipher compiled with" $(CFLAGS)

clean:
	@-rm -f src/*.o
	@-rm -f $(TARGET)

cleanciph:
	@-rm -f src/cipher/*.o
	@-rm -f $(TARGETC)
