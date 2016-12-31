TARGET = mosh
LIBS = -lm
CC = gcc
CFLAGS = -g -Wall -Wextra -Os -pipe -Wpedantic

VERSION = "0.1.0"

.PHONY: default all clean

default: $(TARGET)
all: default

options:
	@echo mosh build options:
	@echo "CC     = ${CC}"
	@echo "CFLAGS = ${CFLAGS}"

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	@echo CC $<
	@$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	@echo CC -o $@
	@$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	@rm -f *.o


mrproper: clean
	@rm -f $(TARGET)
