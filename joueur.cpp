#include <stdlib.h>
#include <time.h>
#include "joueur.h"

using namespace std;

Joueur::Joueur() : pion('/'), num(-1){

}

Joueur::Joueur(char pion, int numJoueur) : pion(pion), num(numJoueur){
}
 
int Joueur::jouer(Plateau &plateau){
	srand(time(NULL));
	int column = rand() % NumberOfColumns;
	while(!plateau.jouer(pion, column)){
		column = rand() % NumberOfColumns;
	}
	return 1;
}

int Joueur::getNum() const{
	return num;
}

void Joueur::setNum(const int num_){
	num = num_;
}

char Joueur::getPion()const{
	return pion;
}

void Joueur::setPion(const char c){
	pion = c;
}

void Joueur::Description(){
	cout << "joueur" << endl;
}