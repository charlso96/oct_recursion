# Makefile for main
CC = gcc
CFLAGS = -g -Wall -Wvla -Werror -Wno-error=unused-variable

MAIN = main

all: $(MAIN)

$(MAIN): $(MAIN).c
		$(CC) $(CFLAGS) -o $(MAIN) $(MAIN).c

clean:
	$(RM) $(MAIN)
