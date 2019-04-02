#include "arbre.h"
#include <ctype.h>

/* cette grammaire est recursicve à droite et ne prend en cimpte correctemetn que les
+ et les *
*/

ARBRE  Expression(char** r);
ARBRE Terme(char** r);
ARBRE Facteur(char** r);
ARBRE  erreur(char** r);
ARBRE Fonction(char** r);
ARBRE NombreOuVariable(char** r);
ARBRE Analyse(char* r);
ARBRE Puissance(char** r);
