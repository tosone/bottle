TARGET    = bottle

CFLAGS   += -Os -std=c99 -Wall -I. -fdata-sections -ffunction-sections -finline-functions
PREFIX   ?= /usr/local

OBJECTS   = $(patsubst %.c, %.o, $(wildcard *.c))

.PHONY: all
all: $(OBJECTS)
	$(CC) -o $(TARGET) $^

.PHONY: clean
clean:
	$(RM) -r *.o *.a *.out $(TARGET) $(TARGET).exe

.PHONY: install
install:
	install -m 755 $(TARGET) $(PREFIX)/bin
