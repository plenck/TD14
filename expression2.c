#include "arbre.h"
#include "traceexp.h"
#include "fonctions.h"

int main(int argc, char** argv) { char s[512];
  double xmin=-5, xmax=5;
  ARBRE r=NULL;
  ARBRE c=NULL;
  int dx=300,dy=200;
  SDL_Surface* f1;

  puts("Entrer ujne expression en notation prefixee"); fgets(s,511,stdin); s[strlen(s)-1]=0;
  r=lireArbre(s);
  puts("Expression :"); afficheArbre(r); puts("");
  /*
      Recopie de r dans c, et liberation de la memoire pour r
      r n'est plus utilisable car n'existe plus
      On trace la copie de r
  */
  c = copieArbre(r);
  r = libereArbre(r);
  puts("L'expression r initiale (doit etre vide)");  afficheArbre(r); puts("");
  puts("L'expression c copiee (doit etre la meme que r au debut)"); afficheArbre(c); puts("");

  if ( (f1=newfenetregraphique(dx,dy)) !=NULL) {
    puts("Entrer les bornes du trace (xmin et xmax): attention au domaine de definition");
    scanf("%lf %lf",&xmin,&xmax);
    trace1courbe(f1,c,dx,xmin,xmax);
    getchar(); getchar();
  }
  c = libereArbre(c);
  SDL_Quit();
  return 0;
}
