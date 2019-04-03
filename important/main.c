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
    tree = lireArbre(s);
    puts("affichage :");
    afficheArbre(tree);
  }
  if(agrc == 2){
    int  n = 0;
    printf("lecture du fichier '%s'\n", agrv[1] );
    FILE *f;
    f = fopen(agrv[1],"r");
    while(!feof(f)){
      n++;
      fgets(s, 500,f);
      puts("");
      tree = lireArbre(s);
      printf("expression %d\n", n);
      afficheArbre(tree);
    }
    fclose(f);
  }
  libereArbre(tree);
  return 0;
}
