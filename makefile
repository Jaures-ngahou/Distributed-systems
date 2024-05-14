CC=gcc
LDFLAGS= -lm -lcurl -lmicrohttpd
ARGS=-g -Wall -pthread  #-pedantic -c
OBJ1= ./obj/client.o ./obj/handling_client.o #./obj/download_client.o
OBJ2= ./obj/serveur.o ./obj/handling_serveur.o ./obj/download_server.o

EXE1=./bin/client/client
EXE2=./bin/serveur/serveur
all: $(EXE1) $(EXE2) 

$(EXE1): $(OBJ1)
	$(CC) $(ARGS) -o $(EXE1) $(OBJ1) $(LDFLAGS)

$(EXE2): $(OBJ2)
	$(CC) $(ARGS) -o $(EXE2) $(OBJ2) $(LDFLAGS)
./obj/handling_client.o: ./src/client/handling_client.c  ./src/client/handling_client.h
	$(CC) $(ARGS) -o ./obj/handling_client.o -c ./src/client/handling_client.c $(LDFLAGS)

./obj/handling_serveur.o: ./src/serveur/handling_serveur.c  ./src/serveur/handling_serveur.h
	$(CC) $(ARGS) -o ./obj/handling_serveur.o -c ./src/serveur/handling_serveur.c

#./obj/download_client.o: ./src/client/download_client.c ./src/client/download_client.h
#	$(CC) %(ARGS) -o ./obj/download_client.o -c ./src/client/download_client.c

./obj/download_server.o: ./src/serveur/download_server.c ./src/serveur/download_server.h
	$(CC) $(ARGS) -o ./obj/download_server.o -c ./src/serveur/download_server.c

./obj/client.o: ./src/client/client.c
	$(CC) $(ARGS) -o ./obj/client.o -c ./src/client/client.c -I. $(LDFLAGS)

./obj/serveur.o: ./src/serveur/serveur.c 
	$(CC) $(ARGS) -o ./obj/serveur.o -c ./src/serveur/serveur.c -I.


clean:
	rm -f ./obj/*
