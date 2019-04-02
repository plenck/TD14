#include "arbre.h"
#ifndef _FONCTION
#define _FONCTION

void afficheArbre(ARBRE r) ;
double evalArbre(ARBRE r, double x) ;
int egalArbre(ARBRE r1, ARBRE r2) ;
ARBRE copieArbre(ARBRE b) ;
ARBRE copieNoeud(ARBRE a, ARBRE b) ;
ARBRE derivArbre(ARBRE r) ;
void afficheInfixe(ARBRE r) ;
#endif
