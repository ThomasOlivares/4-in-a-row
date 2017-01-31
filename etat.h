#ifndef ETAT
#define ETAT

#define POIDS_POSITION 0
#define POIDS_ALIGNEMENT 1
#define BONUS_COUP_POSSIBLE 3

//Poids excessivement grand, marquant la victoire d'un des 2 joueurs
#define POIDS_MAX 1000000

#include <string>
#include <vector>
#include <assert.h>
#include <stdlib.h>
#include "config.h"
#include "plateau.h"

class Etat{
	public :
		Etat(Plateau &plat, Etat* racine_, int numJoueur_, char nextCoup_, int profondeur_);
		~Etat();
		void creationEtatsFils();
		double getPoids() const;
		double getPoidsEnfants() const;
		int getProfondeur();
		void setPoids(const double nouveauPoids);
		int checkGagnant() const;
		int getMeilleurCoup();
		void afficherPoids();
		double avantagePosition(const double row, const double column) const ;

	protected :
		char getNextCoup(const char c);
		void addPoids(const char c, const double p);
		void addPoidsLigne(const char c, const double p, const int row, const int column);
		void gestionAlignement(const int i, const int j, char &actuel,
			 int &consec, const char* type);
		void gestionAlignementLigne(const int i, const int j, char &actuel, int &consec);
		void gestionAlignementDiagonale1(const int i, const int j, char &actuel, 
			int &consec);
		void gestionAlignementDiagonale2(const int i, const int j, char &actuel, 
				int &consec);
		void poidsAlignementDiagonnales();
		double computePoids();
		void remonterPoids(const double nouveauPoids);
		double poidsPosition();
		void poidsAlignement();
		void poidsAlignementLignes();
		void poidsAlignementColonnes();
		
		int profondeur;
		int numJoueur; //IA en train de se servir de l'arbre d'états
		int nextCoup;
		Plateau plateau;
		Etat* racine;
		double poids;
		std::vector<Etat*> enfants;
};

int getMax(const double tab[], const int taille);
int getMin(const double tab[], const int taille);
int getMaxVal(const double tab[], const int taille);
int getMinVal(const double tab[], const int taille);

#endif //ETAT