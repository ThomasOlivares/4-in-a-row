#ifndef MINMAX_H
#define MINMAX_H

#include "joueur.h"
#include "etat.h"
#include "plateau.h"

class MinMax : public Joueur
{

	public:
		MinMax(char pion, int numJoueur_, int profondeur_);
		virtual int jouer(Plateau &plateau);
		virtual void Description();

	protected :
		int profondeur;

};

#endif //MINMAX_H
