#include "arbre.h"
#include <math.h>

void afficheArbre(ARBRE r) {
  switch(r->type){
    case OPERATEUR_BINAIRE :
      printf("(");
      afficheArbre(r->fg);
      printf("%s", r->val);
      afficheArbre(r->fd);
      printf(")");
      break;
    case OPERATEUR_UNAIRE :
      printf("%s(", r->val);
      afficheArbre(r->fg);
      printf(")");
      break;
    case VARIABLE: case VALEUR:
      printf("%s",r->val);
      return;
      break;
  }
  puts("");
  return;
}

double evalArbre(ARBRE r, double x) {
  char *b;
  b=r->val;
  char a = b[0];
  switch(r->type){
    case VALEUR :
      return atof(b);
      break;
    case VARIABLE :
      return x;
      break;
    case OPERATEUR_BINAIRE :
      switch(a){
        case 43 : //43 valeur dec "x"
          return evalArbre(r->fg,x)+evalArbre(r->fd,x);
          break;
        case 42: //valeur dec "*"
          return evalArbre(r->fg,x)*evalArbre(r->fd,x);
          break;
        case 45: //valeur dec "-"
          return evalArbre(r->fg,x)-evalArbre(r->fd,x);
          break;
      }
/*
    case OPERATEUR_UNAIRE :
      switch(a){
        case 115 : //valeur decimale de "s"
          return sin(evalArbre(r->fg,x));
          break;
      }
*/
  }
  printf("qqlch a couillé\n");
  return 0.0;
}

ARBRE copieArbre(ARBRE r) {
  if (r==NULL){
    return NULL;
  }
  else{
    ARBRE a=creerNoeud(r->val);
    a->fg = copieArbre(r->fg);
    a->fd = copieArbre(r->fd);
    return a;
  }
}

ARBRE derivArbre(ARBRE r) {
  ARBRE c;
	if (r!=NULL){
		switch(r -> type){
			case VALEUR :
				return (creerNoeud("0"));
			case VARIABLE :
				return (creerNoeud("1"));
			case OPERATEUR_UNAIRE :
				if(strcasecmp(r -> val,"sin")==0){
					c = creerNoeud("*");
					c -> fg = derivArbre(r -> fg);
					c -> fd = creerNoeud("cos");
					c -> fd -> fg = r -> fg;
					c -> fd -> fd = NULL;
					return c;
				}
				else if(strcasecmp(r -> val,"cos")==0){
					c = creerNoeud("*");
					c -> fg = creerNoeud("-");
					c -> fg -> fg = creerNoeud("0");
					c -> fg -> fd = creerNoeud("1");
					c -> fd = creerNoeud("*");
					c -> fd -> fg = derivArbre(r -> fg);
					c -> fd -> fd = creerNoeud("sin");
					c -> fd -> fd -> fg = r -> fg;
					c -> fd -> fd -> fd = NULL;
					return c;
				}
				else if(strcasecmp(r -> val,"tan")==0){  //1+tan(x)²
					c = creerNoeud("+");
					c -> fg = creerNoeud("1");
					c -> fd = creerNoeud("*");
					c -> fd -> fg = copieArbre(r);
					c -> fd -> fd = copieArbre(r);
					return c;
				}
				else if(strcasecmp(r -> val,"sqrt")==0){ // 1/2*sqrt(x)
					c = creerNoeud("/");
					c -> fg = creerNoeud("1");
					c -> fd = creerNoeud("*");
					c -> fd -> fg = creerNoeud("2");
					c -> fd -> fd = copieArbre(r);
					return c;
				}
			case OPERATEUR_BINAIRE : /*  Une expression binaire*/
				if (r -> val[0]=='+') {  /* C’est un + */
					c=creerNoeud("+");   /* Cration d’un noeud + */
					c -> fg = derivArbre(r -> fg);  /* Copie de la dérivée du fg*/
					c -> fd = derivArbre(r -> fd);  /* Copie de la dérivée du fd*/
					return c;     /* On retourne l’arbre dérivé créé*/
				}
				else if (r -> val[0]=='-') {  /* C’est un - */
					c=creerNoeud("-");   /* Cration d’un noeud + */
					c -> fg = derivArbre(r -> fg);  /* Copie de la dérivée du fg*/
					c -> fd = derivArbre(r -> fd);  /* Copie de la dérivée du fd*/
					return c;     /* On retourne l’arbre dérivé créé*/
				}
				else if (r -> val[0]=='*') {  /* C’est un * */
					c=creerNoeud("+");
					c -> fg = creerNoeud("*");
					c -> fg -> fg = derivArbre(r -> fg);
					c -> fg -> fd = r -> fd;
					c -> fd = creerNoeud("*");
					c -> fd -> fg = r -> fg;
					c -> fd -> fd = derivArbre(r -> fd);
					return c;
				}
				else if (r -> val[0]=='/') {  /* C’est un / */
					c=creerNoeud("/");   /* Cration d’un noeud  */
					c -> fg = creerNoeud("-");
					c -> fg -> fg = creerNoeud("*");
					c -> fg -> fg -> fg = derivArbre(r -> fg);
					c -> fg -> fg -> fd = r -> fd;
					c -> fg -> fd = creerNoeud("*");
					c -> fg -> fd -> fg = r -> fg;
					c -> fg -> fd -> fd = derivArbre(r -> fd);
					c -> fd = creerNoeud("*");
					c -> fd -> fg = r -> fd;
					c -> fd -> fd = r -> fd;
					return c;
				}
		}
	}
  return NULL;
}
