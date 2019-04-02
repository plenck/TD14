#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef _ARBRE
#define _ARBRE

double atof();

typedef enum { OPERATEUR_BINAIRE, OPERATEUR_UNAIRE, VALEUR, VARIABLE } TYPE;
typedef
  struct noeud {
    char * val;
    TYPE type;
    struct noeud* fg, *fd; } NOEUD, * ARBRE;

ARBRE creerNoeud(char *c);
ARBRE construitArbre(char *c) ;
ARBRE lireArbre(char *s) ;
ARBRE libereArbre(ARBRE r) ;
ARBRE libereNoeud(ARBRE r) ;
#endif
