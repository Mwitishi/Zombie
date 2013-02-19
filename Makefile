# File: Makefile / Project: Zombie
# Author: Mwitishi
# Project language: C
# Makefile for project Zombie. Tested under Ubuntu.
# Targets:
# all - Ubuntu compiling, using gcc
# win - Windows compiling, using MinGW under Ubuntu

# Source & header files
CSOURCE=zombie.c zent.c zia.c
CHEADER=zombie.h

#Compilation for Ubuntu
CC=gcc
FLAGS=-Wall -lSDL -lSDL_image
EXE=../bin/zombie

#Compilation for Windows
WINCC=i586-mingw32msvc-gcc
WINFLAGS=-Wall -lmingw32 -lSDLmain -lSDL -lSDL_image
WINEXE=../bin/zombie.exe

all: $(EXE)

$(EXE): $(CSOURCE) $(CHEADER)
	$(CC) $(CSOURCE) $(FLAGS) -o $(EXE)

win: $(WINEXE)

$(WINEXE): $(CSOURCE) $(CHEADER)
	$(WINCC) $(CSOURCE) $(WINFLAGS) -o $(WINEXE)
