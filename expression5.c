#include "arbre.h"
#include "simplifie.h"
#include "traceexp.h"
#include "analyse.h"
#include "fonctions.h"
#define DIM 256



int main(int argc, char** argv) { char s[512];
    char* s2;
  double xmin=-5, xmax=5;
  ARBRE r=NULL, d=NULL, ds=NULL, r2=NULL;
  int dx=300,dy=200;
  SDL_Surface* f1;

  /*
	Lecture d'une expression prefixe au clavier
  */
  puts("Entrer une expression"); fgets(s,511,stdin); s[strlen(s)-1]=0;
  r=Analyse(s);
  //puts("Expression Infixe :"); afficheInfixe(r); puts("");

  /*
	Affichage de cette expression en notation classique
  */
  puts("Expression :"); afficheArbre(r); puts("");

  /*
	Simplification et Affichage de cette expression en notation classique
  */
  puts("Expression simplifiee"); afficheArbre(r2=simplifieArbre(r));


  /*
	Derivation et Affichage de cette derivee en notation classique
        Simplification et Affichage de cette derivee en notation classique
  */
  puts(""); puts("Derivee"); afficheArbre(d=derivArbre(r2)); puts("");
  puts("Derivee simplifiee"); afficheArbre(ds=simplifieArbre(d)); puts("");


  if ( (f1=newfenetregraphique(dx,dy)) != NULL) {
    puts("Entrer les bornes du trace (xmin et xmax): attention au domaine de definition");
    scanf("%lf %lf",&xmin,&xmax);
      /*	Trace des 2 courbes  */
    trace2courbe(f1,r2,ds,dx,xmin,xmax);
    getchar(); getchar();
  }
  SDL_Quit();
  libereArbre(ds);
  libereArbre(r2);
  return 0;
}
