LFLAGS=-lm
CFLAGS=-c -std=c11 -g
FLAGS=-std=c11 -omain -lm -g

all: main

main: main.c dft.o wav_file.o sound.o
	gcc main.c dft.o wav_file.o sound.o $(FLAGS)

dft.o: dft.c
	gcc dft.c $(CFLAGS)

wav_file.o: wav_file.c
	gcc wav_file.c $(CFLAGS)

sound.o: sound.c
	gcc sound.c $(CFLAGS)
