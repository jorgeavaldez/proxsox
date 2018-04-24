TARGET = proxsox
LIBS = -lm -lssl
CC = gcc
CFLAGS = -g -Wall

HEADERS = $(wildcard *.h)
# OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
OBJECTS = proxsox.o

.PHONY: clean all default

default: $(TARGET)
all: default

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
