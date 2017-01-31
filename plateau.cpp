#include "plateau.h"

using namespace std;

Plateau::Plateau(){
	for (int i = 0; i<NumberOfRows; i++){
		for (int j = 0; j<NumberOfColumns; j++){
			grille[i][j] = ' ';
		}
	}
}

Plateau::Plateau(Plateau &plat){
	for (int i = 0; i<NumberOfRows; i++){
		for (int j = 0; j<NumberOfColumns; j++){
			grille[i][j] = plat.get(i, j);
		}
	}
}

Plateau::~Plateau(){
}

void Plateau::display() const{
	for (int i = 0; i<NumberOfRows; i++){
		for (int j = 0; j<NumberOfColumns; j++){
			cout << '|';
			cout << get(i, j);
		}
		cout << '|' << endl;
	}
	for (int j = 0; j<NumberOfColumns; j++){
		cout << "-" << j+1;
	}
	cout << "-" << endl;
}

void Plateau::reboot(){
	for (int i = 0; i<NumberOfRows; i++){
		for (int j = 0; j<NumberOfColumns; j++){
			grille[i][j] = ' ';
		}
	}
}

char Plateau::get(const int row, const int column) const{
	assert(row >= 0);
	assert(row < NumberOfRows);
	assert(column >= 0);
	assert(column < NumberOfColumns);
	return grille[row][column];
}

int Plateau::isValable(const int row, const int column) const{
	return (row >=0) && (row < NumberOfRows) && 
			(column >=0) && (column < NumberOfColumns);
}

int Plateau::getNbPieces() const{
	int nb = 0;
	for (int i = 0; i<NumberOfRows; i++){
		for (int j = 0; j<NumberOfColumns; j++){
			nb+=(grille[i][j] != ' ');
		}
	}
	return nb;
}

int Plateau::getNbVides() const{
	return NumberOfColumns*NumberOfRows - getNbPieces();
}

// indique si la colonne passée en entrée est pleine
int Plateau::isFull(const int column) const{
	assert(column>=0);
	assert(column<=NumberOfColumns);
	return grille[0][column] != ' ';
}

// indique si le plateau est complétement plein
int Plateau::isFull() const{
	for (int column = 0; column<NumberOfColumns; column++){
		if (!isFull(column)){
			return 0;
		}
	}
	return 1;
}

int min(const int a, const int b){
	if (a<=b){
		return a;
	}
	else{
		return b;
	}
}

int max(const int a, const int b){
	if (a<=b){
		return b;
	}
	else{
		return a;
	}
}

int Plateau::jouer(const char c, const int column){
	assert(column>=0);
	assert(column<=NumberOfColumns);
	int row = NumberOfRows - 1;
	while (grille[row][column] != ' ' && row >=0){
		row--;
	}
	if (row == -1){
		return 0;
	}
	grille[row][column] = c;
	return 1;
}

char Plateau::checkRows() const{
	for (int i = 0; i<NumberOfRows; i++){
		int consec = 1;
		char actuel = ' ';
		for (int j = 0; j<NumberOfColumns; j++){
			incrementeConsec(consec, i, j, actuel);
			if (consec == NbCoupGagnant && actuel != ' '){
				return actuel;
			}
		}
	}
	return ' ';
}

char Plateau::checkColumns() const{
	for (int j = 0; j<NumberOfColumns; j++){
		int consec = 1;
		char actuel = ' ';
		for (int i = 0; i<NumberOfRows; i++){
			incrementeConsec(consec, i, j, actuel);
			if (consec == NbCoupGagnant && actuel != ' '){
				return actuel;
			}
		}
	}
	return ' ';
}

char Plateau::checkDiagonals() const{
	for (int cste = 1-NumberOfRows; cste<NumberOfColumns-1 ; cste++){
		int consec = 1;
		char actuel = ' ';
		for (int i = 0; i<NumberOfRows; i++){
			int j = cste + i;
			if (j>=0 && j<NumberOfColumns){
				incrementeConsec(consec, i, j, actuel);
				if (consec == NbCoupGagnant && actuel != ' '){
					return actuel;
				}
			}
		}
	}
	for (int cste = 0; cste<NumberOfColumns + NumberOfRows - 1 ; cste++){
		int consec = 1;
		char actuel = ' ';
		for (int i = 0; i<NumberOfRows-1; i++){
			int j = cste - i;
			if (j>=0 && j<NumberOfColumns){
				incrementeConsec(consec, i, j, actuel);
				if (consec == NbCoupGagnant && actuel != ' '){
					return actuel;
				}
			}
		}
	}
	return ' ';
}

/* Si le caractère lu dans la grille correspond à ceux de l'enchainement précédent, 
on rajoute 1 à l'enchainement, sinon on en crée un nouveau initialisé à 1.
*/
void Plateau::incrementeConsec(int &consec, const int i, const int j, char &actuel) const{
	if (get(i, j) == actuel){
		consec++;
	}
	else{
		consec = 1;
		actuel = get(i, j);
	}
}

char Plateau::etat() const {
	int colonnesPleines = 0;
	for (int i = 0; i< NumberOfColumns; i++){
		colonnesPleines+=isFull(i);
	}
	if (colonnesPleines == NumberOfColumns){
		return 'e';
	}
	char row = checkRows();
	if (row != ' '){
		return row;
	}
	char column = checkColumns();
	if (column != ' '){
		return column;
	}
	char diag = checkDiagonals();
	if (diag != ' '){
		return diag;
	}
	return ' ';
}
