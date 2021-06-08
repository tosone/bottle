TARGET    = bottle

CFLAGS   += -Os -std=c99 -Wall -I.

objects   = $(patsubst %.c, %.o, $(wildcard *.c))

.PHONY: all
all: $(objects)
	$(CC) -o $(TARGET) $^

.PHONY: clean
clean:
	$(RM) -r *.o *.out $(TARGET) $(TARGET).exe
