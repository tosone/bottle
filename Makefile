TARGET    = bottle

CFLAGS   += -Os -std=gnu99 -Wall -I. -fdata-sections -ffunction-sections
CLIBS    += -lm

PREFIX   ?= /usr/local

OBJECTS   = $(patsubst %.c, %.o, $(wildcard *.c))
STRIP     = $(ARCH)strip

.PHONY: all
all: $(OBJECTS)
	$(CC) -o $(TARGET) $^ $(CFLAGS) $(CLIBS)
	$(STRIP) $(TARGET)

.PHONY: clean
clean:
	$(RM) -r *.o *.a *.out $(TARGET) $(TARGET).exe

.PHONY: install
install:
	install -m 755 $(TARGET) $(PREFIX)/bin
