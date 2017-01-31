#include "config.h"

using namespace std;

char complementaire(char pion){
	if(pion == char1){
		return char2;
	}
	else if (pion == char2){
		return char1;
	}
	else{
		cout << "charactere non-valide : config.cpp" << endl;
		exit(EXIT_FAILURE);
	}
}
