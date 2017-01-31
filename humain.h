#ifndef HUMAIN_H
#define HUMAIN_H

#include "joueur.h"
#include "plateau.h"

class Humain : public Joueur
{

	public:
		Humain(char pion, int numJoueur_);
		virtual int jouer(Plateau &plateau);
		virtual void Description();

	protected :


};

#endif //HUMAIN_H
