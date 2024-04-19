CC=gcc
LDFLAGS= -lm
ARGS=-g -Wall -pthread  #-pedantic -c
OBJ= ./obj/client.o
OBJ2= ./obj/serveur.o

EXE1=./bin/client/client
EXE2=./bin/serveur/serveur
all: $(EXE1) $(EXE2) 

$(EXE1): $(OBJ)
	$(CC) $(ARGS) -o $(EXE1) $(OBJ) $(LDFLAGS)

$(EXE2): $(OBJ2)
	$(CC) $(ARGS) -o $(EXE2) $(OBJ2) $(LDFLAGS)

./obj/client.o: ./src/client.c
	$(CC) $(ARGS) -o ./obj/client.o -c ./src/client.c -I.

./obj/serveur.o: ./src/serveur.c 
	$(CC) $(ARGS) -o ./obj/serveur.o -c ./src/serveur.c -I.


clean:
	rm -f ./obj/*
