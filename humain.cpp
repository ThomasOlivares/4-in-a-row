#include "humain.h"

using namespace std;

Humain::Humain(char pion, int numJoueur_): Joueur(pion, numJoueur_){

}

int Humain::jouer(Plateau &plateau){
	int colonne = -1;
	while (colonne <= 0 || colonne >NumberOfColumns || plateau.isFull(colonne-1)){
		cout << "Selectionner un numero de colonne entre 1 et " << NumberOfColumns << endl;
		cin >> colonne;
	}
	return(plateau.jouer(pion, colonne-1));
}

void Humain::Description(){
	cout << "humain" << endl;
}