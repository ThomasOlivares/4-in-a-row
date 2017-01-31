#ifndef TRAIN_H
#define TRAIN_H

#include <iostream>
#include <vector>
#include "neuralNetwork.h"
#include "plateau.h"
#include "joueur.h"
#include "minmax.h"
#include "config.h"

#define nbIA 100
#define nbManches 3
#define nbIterations 10000
#define nbMutations 10
#define AMPLEUR 10

#define TOLERANCE 3

class Train{

	public :
		Train();
		void loop();
		void init();
		void affichage(int &pourcentage);
		void changer(Joueur* actuel) const;
		int jouer(int premierAJouer);
		int tournoi(int iterations);
		int selection();
		void randomShuffle();
		void supprimer(int i);
		void reproduction();
		void reproduction2();
		void clonage();
		void mutation(int debut);
		int test();
		int getMaxVal(const int tab[], const int taille);

	protected :
		Plateau plateau;
		Joueur* joueur1;
		Joueur* joueur2;
		std::vector<NeuralNetwork*> joueurs;
};



#endif //TRAIN_H