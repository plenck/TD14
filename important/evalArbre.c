#include "fonctions.h"
#include "arbre.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
  char str[500];
  ARBRE tree,deriv;
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
  deriv = derivArbre(tree);
  puts("dérivée :");
  afficheArbre(deriv);
  printf("la valeur pour x = %lf est %lf\n",x,y);
}
