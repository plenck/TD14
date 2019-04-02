#include "fonctions.h"
#include "arbre.h"
#include <stdio.h>
#include <stdlib.h>
int main(int agrc,char* agrv[]){
  char s[500];
  ARBRE tree;

  if(agrc ==1 ){
    puts("Entrez une ecriture préfixée au clavier");
    fgets(s, 500, stdin);
    tree = construitArbre(s);
    puts("affichage :");
    afficheArbre(tree);
  }
  if(agrc == 2){
    printf("lecture du fichier '%s' ", agrv[1] );
    FILE *f;
    f = fopen(agrv[1],"r");
    while(!feof(f)){
      fgets(s, 500,f);
      tree = construitArbre(s);
      puts("affichage");
      afficheArbre(tree);
    }
    fclose(f);
  }
  libereArbre(tree);
  return 0;
}
