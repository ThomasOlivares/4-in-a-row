#include <iostream>
#include <string.h>
#include "partie.h"
#include "train.h"

using namespace std;

void test(){
	int const NB_PARTIE = 10000;
	int total = 0;
	int count = 0;
	for (int i = 0; i<NB_PARTIE; i++){
		Partie partie("test", rand()%2);
		char gagnant = partie.jouer();
		if (gagnant == char2){
			count++;
		}
		total++;
		cout << total << "/" << NB_PARTIE << endl;
	}
	cout << "Le systeme intelligent gagne " << (float)count/(float)total*100 << "% du temps" << endl;
}

void jeu(){
	srand(time(NULL));
	Partie partie("jeu", rand()%2);
	partie.jouer();
}

int main(int argc, char* argv[]){
	if (argc <= 1){
		cout << "Vous devez passer 'play' ou 'train' en argument" << endl;
		exit(EXIT_FAILURE);
	}
	
	char* type = argv[1];
	if (strcmp(type, "play") == 0){
		jeu();
	}
	else if (strcmp(type, "train") == 0){
		Train train;
	}
	else{
		cout << "Vous devez passer 'play' ou 'train' en argument" << endl;
		exit(EXIT_FAILURE);
	}
}

