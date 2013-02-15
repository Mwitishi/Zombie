CC=gcc
CSOURCE=zombie.c
CHEADER=zombie.h
FLAGS=-lSDL -lSDL_image
EXE=../bin/zombie

all: $(EXE)

$(EXE): $(CSOURCE) $(CHEADER)
	$(CC) $(CSOURCE) $(FLAGS) -o $(EXE)
