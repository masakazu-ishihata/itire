# Makefile
TAR = test
CC = gcc -O6 -Wall
L = -lm -licsv -lilouds
OBJS = main.o itrie.o
SHAD = -fPIC -shared
LIB = libitrie.so

all : $(TAR) lib
$(TAR): $(OBJS)
	  $(CC) -o $@ $(OBJS) $(L)
clean:
	  rm -f $(OBJS) $(LIB) $(TAR) *~ *.bak

.c.o:	  $(CC) -c $<om

lib:
	$(CC) $(SHAD) -o $(LIB) itrie.c $(L)

main.o: main.h itrie.o
itrie.o: itrie.h
