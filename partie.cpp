#include "partie.h"

using namespace std;

Partie::Partie(string type_, int premierAJouer_) : type(type_), premierAJouer(premierAJouer_){
	init();
}

void Partie::init(){
	plateau;
	joueur1 = new MinMax(char1, 0, 1);
	joueur2 = new NeuralNetwork("neural_network_weight", char2, 1);
}

void Partie::changer(Joueur* actuel) const{
	if (actuel == joueur1){
		actuel = joueur2;
	}
	else{
		actuel = joueur1;
	}

}

char Partie::jouer(){
	init();
	Joueur* actuel;
	if (premierAJouer == 0){
		actuel = joueur1;
	}
	else{
		actuel = joueur2;
	}

	if (type == "jeu"){
		plateau.display();
	}
	
	while(plateau.etat() == ' '){
		actuel->jouer(plateau);
		if (actuel == joueur1){
			actuel = joueur2;
		}
		else{
			actuel = joueur1;
		}
		if (type == "jeu"){
			plateau.display();
		}
	}
	if (type == "jeu"){
		cout << plateau.etat() << " a gagné !!!" << endl;
	}
	return plateau.etat();
}
