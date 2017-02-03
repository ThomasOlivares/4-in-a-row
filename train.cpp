#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "train.h"

using namespace std;

Train::Train(){
	srand(time(NULL));
	loop();
}

void Train::loop(){
	init();
	int pourcentage = 0;
	for (int nb = 0; nb<nbIterations; nb++){
		int nbSurvivants = tournoi(nbManches);
		reproduction2();
		mutation(nbSurvivants);
		affichage(pourcentage, nb);
	}
	tournoi(nbIA);
	cout << joueurs.size() << " IA indépartageables !" << endl;
	NeuralNetwork final = *(joueurs[0]);
	final.sauvegarde("neural_network_weight");
}

void Train::affichage(int &pourcentage, int num){
	if (100*(num+1)/nbIterations > pourcentage){
		int result = test();
		pourcentage++;
		cout << "Generation " << num+1 << "/" << nbIterations << endl;
		if (result > 0){
			cout << "L'IA gagne !!!! : " << result << endl;
		}
		else if (result == 0){
			cout << "Egalité : " << result << endl;
		}
		else{
			cout << "L'IA perd : " << result << endl;
		}
	}
}

void Train::init(){
	plateau;
	for (int i = 0; i<nbIA; i++){
		joueurs.push_back(new NeuralNetwork());
	}
}

void Train::changer(Joueur* actuel) const{
	if (actuel == joueur1){
		actuel = joueur2;
	}
	else{
		actuel = joueur1;
	}
}

/* On joue des parties entre les IA, tous les perdants sont éliminés*/
int Train::tournoi(int iterations){
	for (int j = 0; j<iterations; j++){
		randomShuffle();
		for (int i= 0; i<joueurs.size()-1; i+=2){
			joueur1 = joueurs[i];
			joueur2 = joueurs[i+1];
			joueur1->setNum(0);
			joueur2->setNum(1);
			joueur1->setPion(char1);
			joueur2->setPion(char2);
			int res = jouer(0);
			res += jouer(1);
			if (res > 0){
				supprimer(i+1);
			}	
			else if (res < 0){
				supprimer(i);
			}
		}
	}
	return joueurs.size();
}

int Train::selection(){
	int scores[joueurs.size()];

	joueur2 = new MinMax(char2, 1, 1);
	for (int i = 0; i<joueurs.size(); i++){
		joueur1 = joueurs[i];
		joueur1->setPion(char1);
		joueur1->setNum(0);
		int res = jouer(0);
		res+=jouer(1);
		scores[i] = res;
	}

	int max = this->getMaxVal(scores, joueurs.size());
	int i = 0;
	int j = 0;
	while(i<joueurs.size()){
		if (max-scores[j] > TOLERANCE){
			supprimer(i);
		}
		else{
			i++;
		}
		j++;
	}

	while(joueurs.size() > nbIA/5){
		supprimer(0);
	}
	return joueurs.size();
}

void Train::reproduction(){
	int nbParents = joueurs.size();
	const int nb1 = NeuralNetwork::getNbCouche1();
	const int nb2 = NeuralNetwork::getNbCouche2();
	const int nb3 = NeuralNetwork::getNbCouche3();
	while(joueurs.size() != nbIA){
		double theta1[43][43];
		double theta2[43][7];
		for(int i = 0; i<nb1; i++){
			for (int j = 0; j<nb2; j++){
				int num = rand()%nbParents;
				theta1[i][j] = joueurs[num]->getPoids(1, i, j);
			}
		}
		for(int i = 0; i<nb2; i++){
			for (int j = 0; j<nb3; j++){
				int num = rand()%nbParents;
				theta2[i][j] = joueurs[num]->getPoids(2, i, j);
			}
		}
		joueurs.push_back(new NeuralNetwork(theta1, theta2));
	}
}

// Les valeur de theta1 et theta2 ne sont plus dissossiées.
void Train::reproduction2(){
	int nbParents = joueurs.size();
	const int nb1 = NeuralNetwork::getNbCouche1();
	const int nb2 = NeuralNetwork::getNbCouche2();
	const int nb3 = NeuralNetwork::getNbCouche3();
	while(joueurs.size() != nbIA){
		double theta1[43][43];
		double theta2[43][7];
		int num = rand()%nbParents;
		for(int i = 0; i<nb1; i++){
			for (int j = 0; j<nb2; j++){
				theta1[i][j] = joueurs[num]->getPoids(1, i, j);
			}
		}
		num = rand()%nbParents;
		for(int i = 0; i<nb2; i++){
			for (int j = 0; j<nb3; j++){
				int num = rand()%nbParents;
				theta2[i][j] = joueurs[num]->getPoids(2, i, j);
			}
		}
		joueurs.push_back(new NeuralNetwork(theta1, theta2));
	}
}

// Mutations uniquements
void Train::clonage(){
	int nbParents = joueurs.size();
	const int nb1 = NeuralNetwork::getNbCouche1();
	const int nb2 = NeuralNetwork::getNbCouche2();
	const int nb3 = NeuralNetwork::getNbCouche3();
	while(joueurs.size() != nbIA){
		double theta1[43][43];
		double theta2[43][7];
		int num = rand()%nbParents;
		for(int i = 0; i<nb1; i++){
			for (int j = 0; j<nb2; j++){
				theta1[i][j] = joueurs[num]->getPoids(1, i, j);
			}
		}
		for(int i = 0; i<nb2; i++){
			for (int j = 0; j<nb3; j++){
				theta2[i][j] = joueurs[num]->getPoids(2, i, j);
			}
		}
		joueurs.push_back(new NeuralNetwork(theta1, theta2));
	}
	mutation(nbParents+1);
}

void Train::mutation(int debut){
	for (int i = debut; i<joueurs.size(); i++){
		for (int j = 0; j<nbMutations; j++){
			int modifInt = rand()%(AMPLEUR*100) - rand()%(AMPLEUR*100);
			double modif = (double)modifInt/100;
			int num1 = rand()%nbCouche1;
			int num2 = rand()%nbCouche2;
			joueurs[i]->setPoids(1, num1, num2, modif);
			modif = rand()%AMPLEUR - rand()%AMPLEUR;
			num1 = rand()%nbCouche2;
			num2 = rand()%nbCouche3;
			joueurs[i]->setPoids(2, num1, num2, modif);
		}
	}
}

/* On teste nos intelligences artificielles en les faisant jouer contre
un minmax*/
int Train::test(){
	joueur1 = joueurs[0];
	joueur1->setPion(char1);
	joueur1->setNum(0);
	joueur2 = new MinMax(char2, 1, 1);
	return jouer(0) + jouer(1);
}

void Train::randomShuffle(){
	for (int i = joueurs.size()-1; i>=2; i--){
		int j = rand()%i;
		NeuralNetwork* temp = joueurs[i];
		joueurs[i] = joueurs[j];
		joueurs[j] = temp;
	}
}

void Train::supprimer(int i){
	delete(joueurs[i]);
	for (int j = i; j<joueurs.size()-1; j++){
		joueurs[j] = joueurs[j+1];
	}
	joueurs.pop_back();
}

int Train::jouer(int premierAJouer){
	plateau.reboot();
	Joueur* actuel;
	if (premierAJouer == 0){
		actuel = joueur1;
	}
	else{
		actuel = joueur2;
	}
	
	while(plateau.etat() == ' '){
		// Si l'IA joue dans une colonne pleine, elle perd automatiquement
		if(!actuel->jouer(plateau)){
			return actuel->getNum()*200 - 100;
		}
		if (actuel == joueur1){
			actuel = joueur2;
		}
		else{
			actuel = joueur1;
		}
	}
	if (plateau.etat() == char1){
		return plateau.getNbVides();
	}
	else if (plateau.etat() == 'e'){
		return 0;
	}
	else{
		return -plateau.getNbVides(); 
	}
}

// renvoie le max du tableau
int Train::getMaxVal(const int tab[], const int taille){
	int max = tab[0];
	for (int ind = 1; ind<taille; ind++){
		if (tab[ind] > max){
			max = tab[ind];
		}
	}
	return max;
}
