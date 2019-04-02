#include "arbre.h"

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
  return;
}

double evalArbre(ARBRE r, double x) {
  /* TODO */
  return 0;
}

ARBRE copieArbre(ARBRE b) {
  /* TODO */
  return NULL;
}

ARBRE derivArbre(ARBRE r) {
  /* TODO */
  return NULL;
}
