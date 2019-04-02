#include "arbre.h"
#include "traceexp.h"
#include "fonctions.h"
#include "simplifie.h"

int main(int argc, char** argv) { char s[512];
  double xmin=-5, xmax=5;
  ARBRE r=NULL;
  int dx=400,dy=200;
  SDL_Surface* f1;


  	/*
	Lecture d'une expression prefixe au clavier
 	 */
  puts("Entrer une expression en notation prefixee"); fgets(s,511,stdin); s[strlen(s)-1]=0;
  r=lireArbre(s);
 	 /*
	Affichage de cette expression en notation classique
 	 */
  puts("Expression :"); afficheArbre(r); puts("");
  puts("Expression :"); r=simplifieArbre(r);
  afficheArbre(r); puts("");

  /*
  	Ouverutre d'une fenetre et affichage graphqie
  */
  if ( (f1=newfenetregraphique(dx,dy)) !=NULL) {
    puts("Entrer les bornes du trace (xmin et xmax): attention au domaine de definition");
    scanf("%lf %lf",&xmin,&xmax);
    trace1courbe(f1,r,dx,xmin,xmax);
    getchar(); getchar();
  }

  /*
	Liberation de la memoire de l'arbre
  */
  libereArbre(r);
  r=libereArbre(r);
  SDL_Quit();
  return 0;
}
