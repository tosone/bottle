CFLAGS   += -Os -std=c99 -Wall -I.

objects = $(patsubst %.c, %.o, $(wildcard *.c))

.PHONY: all
all: $(objects)

.PHONY: clean
clean:
	$(RM) -r *.o *.out
