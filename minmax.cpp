#include "minmax.h"

using namespace std;

MinMax::MinMax(char pion, int numJoueur_, int profondeur_): Joueur(pion, numJoueur_),
	 profondeur(profondeur_){

}

int MinMax::jouer(Plateau &plateau){
	char c;
	if (pion == char1){
		c = char2;
	}
	else{
		c = char1;
	}

	Etat* racine = new Etat(plateau, 0, (num+1)%2, c, profondeur);
	//racine->afficherPoids();
	vector<int> columnAvailables;
	for (int i = 0; i<NumberOfColumns; i++){
		if (!plateau.isFull(i)){
			columnAvailables.push_back(i);
		}
	}
	int meilleurCoup = columnAvailables[racine->getMeilleurCoup()];
	delete(racine);
	return(plateau.jouer(pion, meilleurCoup));
}

void MinMax::Description(){
	cout << "minmax" << endl;
}