CSOURCE=zombie.c zent.c
CHEADER=zombie.h

CC=gcc
FLAGS=-Wall -lSDL -lSDL_image
EXE=../bin/zombie

WINCC=i586-mingw32msvc-gcc
WINFLAGS=-Wall -lmingw32 -lSDLmain -lSDL -lSDL_image
WINEXE=../bin/zombie.exe

all: $(EXE)

$(EXE): $(CSOURCE) $(CHEADER)
	$(CC) $(CSOURCE) $(FLAGS) -o $(EXE)

win: $(WINEXE)

$(WINEXE): $(CSOURCE) $(CHEADER)
	$(WINCC) $(CSOURCE) $(WINFLAGS) -o $(WINEXE)
