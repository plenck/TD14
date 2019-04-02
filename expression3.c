#include "traceexp.h"
#include "arbre.h"
#include "simplifie.h"
#include "fonctions.h"



int main(int argc, char** argv) { char s[512];
  double xmin=-5, xmax=5;
  ARBRE r=NULL, d=NULL, ds=NULL, rs=NULL;
  int dx=300,dy=200;
  SDL_Surface* f1;

  /*
	Lecture d'une expression prefixe au clavier
  */
  puts("Entrer une expression"); fgets(s,511,stdin); s[strlen(s)-1]=0;
  rs=lireArbre(s);

  /*
	Affichage de cette expression en notation classique
  */
  puts("Expression :"); afficheArbre(rs); puts("");

  /*
	Simplification et Affichage de cette expression en notation classique
  */
  puts("Expression simplifiee"); afficheArbre(r=simplifieArbre(rs));


  /*
	Derivation et Affichage de cette derivee en notation classique
        Simplification et Affichage de cette derivee en notation classique
  */
  puts(""); puts("Derivee"); afficheArbre(d=derivArbre(r)); puts("");
  puts("Derivee simplifiee"); afficheArbre(ds=simplifieArbre(derivArbre(r))); puts("");


  if ( (f1=newfenetregraphique(dx,dy)) != NULL) {
    puts("Entrer les bornes du trace (xmin et xmax): attention au domaine de definition");
    scanf("%lf %lf",&xmin,&xmax);
        /*	       Trace des 2 courbes          */
    trace2courbe(f1,r,d,dx,xmin,xmax);
    getchar(); getchar();
  }
  libereArbre(r);
  libereArbre(ds);
  libereArbre(d);
  SDL_Quit();
  return 0;
}
