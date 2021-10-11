CC = gcc
CFLAGS = -std=gnu99 -Wall -g #-DDEBUG

EXECS = beangame-client beangame-server

all: $(EXECS)

wrappers.o: wrappers.h wrappers.c
	$(CC) $(CFLAGS) -c wrappers.c

beangame-client: beangame-client.c wrappers.o
	$(CC) $(CFLAGS) beangame-client.c wrappers.o -o $@

beangame-server: beangame-server.c wrappers.o
	$(CC) $(CFLAGS) beangame-server.c wrappers.o -o $@

.PHONY: clean
clean:
	/bin/rm -rf $(EXECS) wrappers.o
