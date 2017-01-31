#include "etat.h"

using namespace std;

/*
plat : plateau associé à l'état
racine : etat du coup précédent
numJoueur : numero du joueur qui va jouer
*/
Etat::Etat(Plateau &plat, Etat* racine_, int numJoueur_, char nextCoup_, int profondeur_) : 
		plateau(plat), racine(racine_), poids(0), numJoueur(numJoueur_), nextCoup(nextCoup_),
		profondeur(profondeur_){
	if (plateau.getNbPieces() >= 32){
		profondeur = -1;
	}
	/*
	if (racine == 0){
		cout << "Poids du système : " << getPoids() << endl;
	}
	*/

	// Si la condition est vraie, l'état est une "feuille" de l'arbre
	if (plateau.etat() != ' ' || profondeur == 0){
		computePoids();
	}
	else{
		creationEtatsFils();
	}
	remonterPoids(poids);
}

Etat::~Etat(){
	for (int i = 0; i<enfants.size(); i++){
		assert(enfants[i]);
		delete(enfants[i]);   
	}
}

void Etat::creationEtatsFils(){
	// Ajout d'un poids excessivement bon afin qu'il soit remplacé quelle que soit la situation
	addPoids(nextCoup, 2*POIDS_MAX);

	char nextCoup_2 = getNextCoup(nextCoup);
	
	// Test de tous les coup possibles et lancement récursifs des états correspondants
	for (int column = 0; column < NumberOfColumns; column ++){
		Plateau next(plateau);
		if (next.jouer(nextCoup, column)){
			Etat* nouveau = new Etat(next, this, (numJoueur+1)%2, nextCoup_2, profondeur-1);
			enfants.push_back(nouveau);
		}
	}	
}

void Etat::afficherPoids(){
	for (int i = 0; i<enfants.size(); i++){
		cout << enfants[i]->getPoids() << " / ";
	}
	cout << endl;
}

// Renvoie le pion opposé à celui passé en paramètre
char Etat::getNextCoup(const char c){
	if (c == char1){
		nextCoup = char2;
	}
	else{
		nextCoup = char1;
	}
	return nextCoup;
}

int Etat::getProfondeur(){
	return profondeur;
}

double Etat::getPoids() const{
	return poids;
}

void Etat::setPoids(const double nouveauPoids){
	poids = nouveauPoids;
}

double Etat::computePoids(){
	poids = checkGagnant();
	if (poids == 0){
		poids+=poidsPosition();
		poidsAlignement();
	}
	return poids;
}

/* Si le poids de cet état est plus défavorable pour le joueur
que le poids actuel, on le remplace par celui ci.
Ceci permet de toujours considérer le cas le plus 
défavorable où l'adversaire joue parfaitement.
*/
void Etat::remonterPoids(const double nouveauPoids){
	// Pas de remontée quand on est à la racine
	if (racine == 0){
		return;
	}

	if (numJoueur == 0 && nouveauPoids > racine->getPoids()){
		racine->setPoids(nouveauPoids);
	}
	else if (numJoueur == 1 && nouveauPoids < racine->getPoids()){
		racine->setPoids(nouveauPoids);
	}
}

/* Si un des 2 joueurs gagne, le poids prend la valeur max
Afin de distinguer des cas tels que : le joueur gagne dans 1/5/10 coups
on retranche le nombre de pièce afin de valoriser des victoires rapides
*/
int Etat::checkGagnant() const{
	if (plateau.etat() == char1){
		return POIDS_MAX - plateau.getNbPieces();
	}
	else if (plateau.etat() == char2){
		return -POIDS_MAX + plateau.getNbPieces();
	}
	return 0;
}

// Ajoute le signe + ou - devant un poids selon si on l'évalue pour le joueur 1 ou 2
void Etat::addPoids(const char c, const double p){
	assert((c==char1) || (c==char2) || (c==' '));
	if (c == char1){
		poids += p;
	}
	else if (c==char2){
		poids -= p;
	}
}

// On distingue les cas ou on peut immediatement allonger un alignement, et le cas inverse
void Etat::addPoidsLigne(const char c, const double p, const int row, 
		const int column){
	if ((row == NumberOfRows - 1)){
		addPoids(c, BONUS_COUP_POSSIBLE*p);
	}
	else if (plateau.get(row+1, column) != ' '){
		addPoids(c, BONUS_COUP_POSSIBLE*p);
	}
	else{
		addPoids(c, p);
	}
}

// Calcule la fonction de cout lié uniquement à la position de chaque pion
double Etat::poidsPosition(){
	double total = 0;
	for (int i = 0; i<NumberOfRows; i++){
		for (int j = 0; j<NumberOfColumns; j++){
			addPoids(plateau.get(i, j), POIDS_POSITION*avantagePosition(i, j));
		}
	}
	return total;
}

void Etat::poidsAlignement(){
	poidsAlignementLignes();
	poidsAlignementColonnes();
	poidsAlignementDiagonnales();
}

void Etat::poidsAlignementLignes(){
	for (int i = 0; i<NumberOfRows; i++){
		int consec = 1;
		char actuel = ' ';
		for (int j = 0; j<NumberOfColumns; j++){
			gestionAlignement(i, j, actuel, consec, "ligne");
		}
	}
}

void Etat::gestionAlignement(const int i, const int j, char &actuel, 
		int &consec, const char* type){
	if (type == "ligne"){
		gestionAlignementLigne(i, j, actuel, consec);
	}
	else if (type == "diagonale1"){
		gestionAlignementDiagonale1(i, j, actuel, consec);
	}
	else if (type == "diagonale2"){
		gestionAlignementDiagonale2(i, j, actuel, consec);
	}
}

void Etat::gestionAlignementLigne(const int i, const int j, char &actuel, 
		int &consec){
	int condArret = (j == NumberOfColumns-1);
	if (plateau.get(i, j) == actuel){
		consec++;
	}
	else{
		char precedent = actuel;
		actuel = plateau.get(i, j);

		// On ne valorise un alignement que si au moins une de ses deux extrémités est libre
		if (plateau.isValable(i, j-consec-1)){
			if (plateau.get(i, j-consec-1) == ' '){
				addPoidsLigne(precedent, POIDS_ALIGNEMENT*consec*consec, i, j-consec-1);
			}
		}
		if (actuel == ' '){
			addPoidsLigne(precedent, POIDS_ALIGNEMENT*consec*consec, i, j);
		}
		consec = 1;
	}
	if (condArret && plateau.isValable(i, j-consec)){
		if (plateau.get(i, j-consec) == ' '){
			addPoidsLigne(actuel, POIDS_ALIGNEMENT*consec*consec, i, j-consec);
		}
	}
}

void Etat::gestionAlignementDiagonale1(const int i, const int j, char &actuel, 
		int &consec){
	int	condArret =  (i == NumberOfRows-1) || (j == NumberOfColumns-1);
	if (plateau.get(i, j) == actuel){
		consec++;
	}
	else{
		char precedent = actuel;
		actuel = plateau.get(i, j);

		// On ne valorise un alignement que si au moins une de ses deux extrémités est libre
		if (plateau.isValable(i-consec-1, j-consec-1)){
			if (plateau.get(i-consec-1, j-consec-1) == ' '){
				addPoidsLigne(precedent, POIDS_ALIGNEMENT*consec*consec, 
					i-consec-1, j-consec-1);
			}
		}
		if (actuel == ' '){
			addPoidsLigne(precedent, POIDS_ALIGNEMENT*consec*consec, i, j);
		}
		consec = 1;
	}
	if (condArret && plateau.isValable(i-consec, j-consec)){
		if (plateau.get(i-consec, j-consec) == ' '){
			addPoidsLigne(actuel, POIDS_ALIGNEMENT*consec*consec, i-consec, j-consec);
		}
	}
}

void Etat::gestionAlignementDiagonale2(const int i, const int j, char &actuel, 
		int &consec){
	int condArret =  (i == NumberOfRows-1) || (j == 0);
	if (plateau.get(i, j) == actuel){
		consec++;
	}
	else{
		char precedent = actuel;
		actuel = plateau.get(i, j);

		// On ne valorise un alignement que si au moins une de ses deux extrémités est libre
		if (plateau.isValable(i-consec-1, j+consec+1)){
			if (plateau.get(i-consec-1, j+consec+1) == ' '){
				addPoidsLigne(precedent, POIDS_ALIGNEMENT*consec*consec, 
					i-consec-1,  j+consec+1);
			}
		}
		if (actuel == ' '){
			addPoidsLigne(precedent, POIDS_ALIGNEMENT*consec*consec, i, j);
		}
		consec = 1;
	}
	if (condArret && plateau.isValable(i-consec, j+consec)){
		if (plateau.get(i-consec, j+consec) == ' '){
			addPoidsLigne(actuel, POIDS_ALIGNEMENT*consec*consec, i-consec, j+consec);
		}
	}
}

void Etat::poidsAlignementDiagonnales(){
	for (int cste = 1-NumberOfRows; cste<NumberOfColumns-1 ; cste++){
		int consec = 1;
		char actuel = ' ';
		for (int i = 0; i<NumberOfRows; i++){
			int j = cste + i;
			if (j>=0 && j<NumberOfColumns){
				gestionAlignement(i, j, actuel, consec, "diagonale1");
			}
		}
	}
	for (int cste = 0; cste<NumberOfColumns + NumberOfRows - 1 ; cste++){
		int consec = 1;
		char actuel = ' ';
		for (int i = 0; i<NumberOfRows; i++){
			int j = cste - i;
			if (j>=0 && j<NumberOfColumns){
				gestionAlignement(i, j, actuel, consec, "diagonale2");
			}
		}
	}
}

void Etat::poidsAlignementColonnes(){
	for (int j = 0; j<NumberOfColumns; j++){
		int consec = 1;
		char actuel = ' ';
		for (int i = NumberOfRows-1; i>=0; i--){
			if (plateau.get(i, j) == actuel){
				consec++;
			}
			else{
				char precedent = actuel;
				actuel = plateau.get(i, j);
				// On ne valorise un alignement que si au moins une de ses deux extrémités est libre
				if (actuel == ' '){
					addPoids(precedent, POIDS_ALIGNEMENT*BONUS_COUP_POSSIBLE*consec*consec);
				}
				consec = 1;
			}
		}
	}
}

/* Fonction évaluant l'impact d'un pion en fonction de sa position uniquement
   plus il est au centre, plus le resultat sera grand : [epsilon² ; 1]*/
double Etat::avantagePosition(const double row, const double column) const {
	double res = 0;
	double epsilon = 0.1;
	if (row < NumberOfRows/2){
		res += 2*(1-epsilon)*row/NumberOfRows + epsilon;
	}
	else{
		res += 2*(1-epsilon)*(1-row/NumberOfRows) + epsilon;
	}
	if (column < (NumberOfColumns-1)/2){
		res *= 2*(1-epsilon)*column/(NumberOfColumns-1) + epsilon;
	}
	else{
		res *= 2*(1-epsilon)*(1-column/(NumberOfColumns-1)) + epsilon;
	}

	// Plus la partie avance et moins la position a d'importance
	double nbEmplacement = NumberOfColumns*NumberOfRows-plateau.getNbPieces();
	double coeff = nbEmplacement/4;

	return res*coeff;
}

/* Fonction donnant la colonne donnant le meilleur résulat
Attention : le numero renvoyé est à considéré sur l'ensemble des colonne non - pleine
Ex : Si la colonne 5 est la seule non-pleine, la fonction renverra 0;
Ne peut être appelée que pour la racine de l'arbre
*/
int Etat::getMeilleurCoup(){
	assert(racine==0);

	double poidsEnfants[enfants.size()];
	for (int i = 0; i< enfants.size(); i++){
		poidsEnfants[i] = enfants[i]->getPoids();
	}
	if (numJoueur == 0){
		return getMin(poidsEnfants, enfants.size());
	}
	else{
		return getMax(poidsEnfants, enfants.size());
	}
}

// renvoie l'indice du maximum du tableau
int getMax(const double tab[], const int taille){
	int max = 0;
	for (int ind = 1; ind<taille; ind++){
		if (tab[ind] > tab[max]){
			max = ind;
		}
	}
	return max;
}

int getMin(const double tab[], const int taille){
	int min = 0;
	for (int ind = 1; ind<taille; ind++){
		if (tab[ind] < tab[min]){
			min = ind;
		}
	}
	return min;
}

// renvoie l'indice du maximum du tableau
int getMaxVal(const double tab[], const int taille){
	int max = tab[0];
	for (int ind = 1; ind<taille; ind++){
		if (tab[ind] > tab[max]){
			max = tab[ind];
		}
	}
	return max;
}

int getMinVal(const double tab[], const int taille){
	int min = tab[0];
	for (int ind = 1; ind<taille; ind++){
		if (tab[ind] < tab[min]){
			min = tab[ind];
		}
	}
	return min;
}
