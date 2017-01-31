#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "neuralNetwork.h"

using namespace std;

NeuralNetwork::NeuralNetwork() : Joueur(){
	initPoids();
}

NeuralNetwork::NeuralNetwork(double theta1_[][nbCouche2], 
	double theta2_[][nbCouche3]) : Joueur(){
	initPoids(theta1_, theta2_);
}

NeuralNetwork::NeuralNetwork(char* pathname, char pion, int numJoueur) : 
		Joueur(pion, numJoueur){
	FILE* f = fopen(pathname, "r");
	fread(theta1, sizeof(double)*nbCouche1*nbCouche2, 1, f);
	fread(theta2, sizeof(double)*nbCouche2*nbCouche3, 1, f);
	fclose(f);
}

double NeuralNetwork::getPoids(int numCouche, int i, int j){
	if (numCouche == 1){
		return theta1[i][j];
	}
	else if (numCouche == 2){
		return theta2[i][j];
	}
	else{
		cout << "getPoids : numéro de couche non-valide" << endl;
		exit(EXIT_FAILURE);
	}
}

void NeuralNetwork::setPoids(int numTheta, int i, int j, double modif){
	if (numTheta == 1){
		theta1[i][j] += modif;
	}
	else if (numTheta == 2){
		theta2[i][j] += modif;
	}
	else{
		cout << "setPoids : numéro de couche non-valide" << endl;
		exit(EXIT_FAILURE);
	}
}

void NeuralNetwork::initCouche1(Plateau &plateau){
	int ind = 0;
	for (int i = 0; i<NumberOfRows; i++){
		for (int j = 0; j<NumberOfColumns; j++){
			if (plateau.get(i, j) == char1){
				couche1[ind] = 1;
			}
			else if (plateau.get(i, j) == char2){
				couche1[ind] = -1;
			}
			else{
				couche1[ind] = 0;
			}
			ind++;
		}
	}
	couche1[nbCouche1-1] = 1;
}

void NeuralNetwork::initPoids(){
	for(int i = 0; i<nbCouche1; i++){
		for (int j = 0; j<nbCouche2; j++){
			theta1[i][j] = (double)(rand()%10000)/10000;
		}
	}
	for(int i = 0; i<nbCouche2; i++){
		for (int j = 0; j<nbCouche3; j++){
			theta2[i][j] = (double)(rand()%10000)/10000;
		}
	}
}

void NeuralNetwork::initPoids(double theta1_[][nbCouche2], double theta2_[][nbCouche3]){
	for(int i = 0; i<nbCouche1; i++){
		for (int j = 0; j<nbCouche2; j++){
			theta1[i][j] = theta1_[i][j];
		}
	}
	for(int i = 0; i<nbCouche2; i++){
		for (int j = 0; j<nbCouche3; j++){
			theta2[i][j] = theta2_[i][j];
		}
	}
}

void NeuralNetwork::computeCouche2(){
	for (int i = 0; i<nbCouche2-1; i++){
		couche2[i] = 0;
		for (int j = 0; j<nbCouche1; j++){
			couche2[i] += (double)couche1[j] * theta1[j][i];
		}
		couche2[i] = sigmoid(couche2[i]);
	}
	couche2[nbCouche2-1] = 1;
}

void NeuralNetwork::computeCouche3(){
	for (int i = 0; i<nbCouche3; i++){
		couche3[i] = 0;
		for (int j = 0; j<nbCouche2; j++){
			couche3[i] += (double)couche2[j] * theta2[j][i];
		}
		couche3[i] = sigmoid(couche3[i]);
	}
}

int NeuralNetwork::evaluation(){
	int max = 0;
	for (int i = 0; i<nbCouche3; i++){
		if (couche3[i]>couche3[max]){
			max = i;
		}
	}
	return max;
}

void NeuralNetwork::afficherPoids(){
	cout << "theta 1 : " << endl;
	for (int i= 0; i<nbCouche1; i++){
		for (int j = 0 ; j<nbCouche2; j++){
			cout << theta1[i][j] << " / ";
		}
	}
	cout << endl;
	cout << "theta 2 : " << endl;
	for (int i= 0; i<nbCouche2; i++){
		for (int j = 0 ; j<nbCouche3; j++){
			cout << theta2[i][j] << " / ";
		}
	}
	cout << endl;
}

int NeuralNetwork::getNbCouche1(){
	return nbCouche1;
}

int NeuralNetwork::getNbCouche2(){
	return nbCouche2;
}

int NeuralNetwork::getNbCouche3(){
	return nbCouche3;
}

int NeuralNetwork::jouer(Plateau &plateau){
	srand(time(NULL));
	initCouche1(plateau);
	computeCouche2();
	computeCouche3();
	return(plateau.jouer(pion, evaluation()));
}

void NeuralNetwork::sauvegarde(char* name){
	FILE* f = fopen(name, "w");
	fwrite(theta1, sizeof(double)*nbCouche1*nbCouche2, 1, f);
	fwrite(theta2, sizeof(double)*nbCouche2*nbCouche3, 1, f);
	fclose(f);
}


double sigmoid(double z){
	return 1/(1+exp(z));
}

void NeuralNetwork::Description(){
	cout << "réseau de neurones" << endl;
}