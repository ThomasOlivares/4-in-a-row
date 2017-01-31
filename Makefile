FLAG=main.o \
	plateau.o \
	config.o \
	partie.o \
	joueur.o \
	minmax.o \
	neuralNetwork.o \
	etat.o \
	humain.o \
	train.o

C_FLAG=-Wall -Werror

all : main

main : $(FLAG)
	g++ $(FLAG) -o puissance4 $(C_FLAG)

main.o : main.cpp
	g++ -c main.cpp

plateau.o : plateau.cpp
	g++ -c plateau.cpp

config.o : config.cpp
	g++ -c config.cpp

etat.o : etat.cpp
	g++ -c etat.cpp

humain.o : humain.cpp
	g++ -c humain.cpp

neuralNetwork.o : neuralNetwork.cpp
	g++ -c neuralNetwork.cpp

partie.o : partie.cpp
	g++ -c partie.cpp

joueur.o : joueur.cpp
	g++ -c joueur.cpp

minmax.o : minmax.cpp
	g++ -c minmax.cpp

train.o : train.cpp
	g++ -c train.cpp

clean : 
	rm *.o
