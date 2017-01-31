#ifndef JOUEUR_H
#define JOUEUR_H

#include "plateau.h"
#include <iostream>

class Joueur{

	public :
		Joueur();
		Joueur(char pion, int numJoueur);
		int getNum() const;
		void setNum(const int num_);
		char getPion()const;
		void setPion(const char c);
		virtual int jouer(Plateau &plateau);
		virtual void Description();

	protected :
		char pion;
		int num;
};

#endif //JOUEUR_H