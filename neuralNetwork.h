#ifndef NEURAL_NETWORK
#define NEURAL_NETWORK

#include "plateau.h"
#include "config.h"
#include "joueur.h"

#define nbCouche 3
#define nbCouche1 43
#define nbCouche2 43
#define nbCouche3 7

class NeuralNetwork : public Joueur{
	public :
		NeuralNetwork();
		NeuralNetwork(double theta1_[][nbCouche2], double theta2_[][nbCouche3]);
		NeuralNetwork(char* pathname, char pion, int numJoueur);
		double getPoids(int numCouche, int i, int j);
		void setPoids(int numTheta, int i, int j, double modif);
		void initCouche1(Plateau &plateau);
		void initPoids();
		void initPoids(double theta1_[][nbCouche2], double theta2_[][nbCouche3]);
		void computeCouche2();
		void computeCouche3();
		int evaluation();
		void afficherPoids();
		static int getNbCouche1();
		static int getNbCouche2();
		static int getNbCouche3();
		virtual int jouer(Plateau &plateau);
		virtual void Description();
		void sauvegarde(char* name);

	protected :
		int couche1[nbCouche1];
		double couche2[nbCouche2];
		double couche3[nbCouche3];
		double theta1[nbCouche1][nbCouche2];
		double theta2[nbCouche2][nbCouche3];
};

double sigmoid(double z);


#endif // NEURAL_NETWORK