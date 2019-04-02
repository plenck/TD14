#include "arbre.h"

/*
  Fonction de creation d'un noeud a partir d'une chaine de caracteres contenant:
  Role :
     - alloue un espace memoire n�cessaire a un oeoud
     - alloue et copie la chaine dans le noeud (champ val)
     - met a jour le type du noeud en fonction de la chaine
          - variable
          - constante
          - operateur unaire (sin, cos, tan, sqrt, ...)
          - operateur binaire (+,-,*,/,^^)
*/
ARBRE libereNoeud(ARBRE a) { free(a->val); free(a); return NULL;}

ARBRE creerNoeud(char *c) {
  ARBRE r=NULL; double x;

  if (c==NULL || *c=='\0') { puts("Erreur de syntaxe"); exit(1); }
	/* Allocation memoire d'un noeud */
  r=calloc(1,sizeof(*r));
  if (r==NULL) { puts("Plus assez de memoire "); exit(1); }
	/* Copie de la chaine sur le noeud */
  r->val=strdup(c);
        /* Recherche du type de noeud */
	/* Est ce un nombre ? */
  if (sscanf(c,"%lf",&x)==1) { r->type=VALEUR; return r;}
  switch(*c) {
	/* Est ce un operateur classique? */
    case '+' : case '-': case '*': case '/' : case'^':
         r->type=OPERATEUR_BINAIRE; return r;
	/* Est ce une variable ? */
    case 'x': case 'y' :
          r->type=VARIABLE; return r;
  }
	/* Est ce une fonction connue ? */
  if (!strcasecmp(c,"sin") || !strcasecmp(c,"cos") || !strcasecmp(c,"tan") || !strcasecmp(c,"sqrt")
      || !strcasecmp(c,"exp") || !strcasecmp(c,"log")) { r->type=OPERATEUR_UNAIRE; return r; }

  if (!strcasecmp(c,"u-")){ r->type=OPERATEUR_UNAIRE; strcpy(r->val,"-"); return r; }

  printf("%s : fonction non implantee\n",c);
  free(r);
  exit(EXIT_FAILURE);
}

/*
   Focntion construisant un arbre a partir d'une chaine decoupee en mots

   Algo :
     recupere le premier mot de la chaine par la fonction strtok(NULL," ")
     si ce mot est
       - une constante ou une variable : construit et retourne le noued contenant cette constante ou cette variable
       - un operateur unaire :
               construit le noued contenant cet operateur;
               Le fils droit de ce noeud est vide.
               le fils gauche de ce noeud est le sous arbre obtenu par le reste de l'expression en cours d'analyse
               retourne le noued ainsi obtenu
       - un operateur binaire :
               construit le noued contenant cet operateur;
               le fils gauche de ce noeud est le sous arbre obtenu par le reste de l'expression en cours d'analyse. Cette analyse s'arrete a la fin de l'expression contenant le fils gauche car les expressions sont bien construites
               Le fils droit de ce noeud est le sous arbre obtenu par le reste de l'expression en cours d'analyse
               retourne le noued ainsi obtenu
*/

ARBRE construitArbre(char *c) {
  ARBRE r=NULL;
  r=creerNoeud(c);
  switch(r->type) {
    case OPERATEUR_BINAIRE:
         r->fg=construitArbre(strtok(NULL," ")); r->fd=construitArbre(strtok(NULL," "));
         return r;
    case OPERATEUR_UNAIRE :
         r->fd=construitArbre(strtok(NULL," ")); return r;
    case VARIABLE: case VALEUR:
         return r;
    default : libereNoeud(r); return NULL;
  }
  return NULL;
}

/*
  Fonction qui construit un arbre a partir d'une expression.
  Elle decoupe la chaine en suite de mot (fonction strtok). Les appels suivants a strtok permettront de recuperer les mots les uns apres les autres.
  Elle appelle ensuite la creation d'un arbre a partir d'une suite de mots
*/
ARBRE lireArbre(char *s) {
  char *c = strtok(s," ");
  return construitArbre(c);
}

/*
  Fonction qui libere la memoire alloue pour un arbre
*/
ARBRE libereArbre(ARBRE r) {
  if (r!=NULL) { r->fg=libereArbre(r->fg); r->fd=libereArbre(r->fd);
    if (r->val) free(r->val);
    free(r);
  }
  return NULL;
}
