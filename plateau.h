#ifndef PLATEAU_H
#define PLATEAU_H

#include <iostream>
#include <assert.h>
#include "config.h"

class Plateau{

	public : 
		Plateau();
		Plateau(Plateau &plat);
		~Plateau();

		void display() const;
		void reboot();
		char get(const int row, const int column) const;
		int isValable(const int row, const int column)const;
		int getNbPieces() const;
		int getNbVides() const;
		int isFull(const int column) const;
		int isFull() const;
		int jouer(const char c, const int column);

		char etat() const;

	protected : 
		char checkRows() const;
		char checkColumns() const;
		char checkDiagonals() const;
		void incrementeConsec(int &consec, const int i, const int j, char &actuel) const;
		
		char grille[NumberOfRows][NumberOfColumns];
};

int min(const int a, const int b);
int max(const int a, const int b);

#endif //PLATEAU_H
