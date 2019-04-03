#include "fonctions.h"
#include "arbre.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
  char str[500];
  ARBRE tree;
  double x = 0;
  double y = 0;
  puts("Entrez une ecriture préfixée au clavier");
  fgets(str, 500, stdin);
  tree = lireArbre(str);
  puts("Entrez la valeur de x :");
  fgets(str, 500, stdin);
  x = atof(str);
  puts("affichage :");
  afficheArbre(tree);
  y = evalArbre(tree, x);
  printf("la valeur pour x = %lf est %lf\n",x,y);
}
