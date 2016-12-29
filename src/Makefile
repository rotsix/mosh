TARGET = mosh
LIBS = -lm
CC = gcc
CFLAGS = -g -Wall -Wextra -Os -pipe -Wpedantic

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	@rm -f *.o


mrproper: clean
	@rm -f $(TARGET)
