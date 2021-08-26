TARGET    = bottle

CC       ?= $(CROSS_COMPILE)gcc
CFLAGS   += -Os -std=gnu99 -Wall -I./include -fdata-sections -ffunction-sections
CLIBS    += -lm

PREFIX   ?= /usr/local

OBJECTS   = $(patsubst %.c, %.o, $(wildcard src/*.c))
STRIP     = $(CROSS_COMPILE)strip

.PHONY: all
all: $(OBJECTS)
	$(CC) -o $(TARGET) $^ $(CFLAGS) $(CLIBS)

.PHONY: clean
clean:
	$(RM) -r src/*.o *.o *.a *.out $(TARGET) $(TARGET).exe

.PHONY: install
install:
	install -m 755 $(TARGET) $(PREFIX)/bin
