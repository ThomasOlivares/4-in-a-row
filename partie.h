#ifndef PARTIE_H
#define PARTIE_H

#include <iostream>
#include "humain.h"
#include "minmax.h"
#include "neuralNetwork.h"
#include "plateau.h"
#include "joueur.h"
#include "config.h"

class Partie{

	public :
		Partie(std::string type_, int premierAJouer_);
		void init();
		void changer(Joueur* actuel) const;
		char jouer();

	protected :
		int premierAJouer;
		std::string type;
		Plateau plateau;
		Joueur* joueur1;
		Joueur* joueur2;
};

#endif //PARTIE_H