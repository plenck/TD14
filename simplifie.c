//
//  simplifie.c
//
//
//  Created by Michel Desvignes on 09/03/2017.
//
//
#include "arbre.h"
#include "fonctions.h"
#include "simplifie.h"
ARBRE simplifie1(ARBRE r);
ARBRE simpl2(ARBRE r);
ARBRE distribution( ARBRE r) ;
char* opNoeud(char* a1,char* sv, char op) ;
ARBRE ajouteSousArbre(ARBRE a, char* r, char typea, char typer) ;
int egalArbre(ARBRE r1, ARBRE r2);
char* double2string(double x, char* s) ;

int egalArbre(ARBRE r1, ARBRE r2) {
  if (r1==NULL)
    if (r2!=NULL) return 0;
    else return 1;
  else
    if (r2==NULL) return 0;
    else if (strcmp(r1->val,r2->val)==0) {
            /* 2 arbres egaux exactemetn ou avec commutativite pour + et * */
            char res=egalArbre(r1->fg,r2->fg) && egalArbre(r1->fd,r2->fd);
            return ( (r1->val[0]=='+'||r1->val[0]=='*') ? res || (egalArbre(r1->fg,r2->fd) && egalArbre(r1->fd,r2->fg)) : res );
          }
          else return 0;
}

char* double2string(double x, char* s) {int i;
  sprintf(s,"%lf",x);
  for(i=strlen(s)-1; i>0 && s[i]=='0'; i--) s[i]=0;
  if (i>0 && s[i]=='.')s[i]=0;
  return s;
}

/* Chaine resultat de l'addition de 2 neouds */
char* opNoeud(char* a1,char* sv, char op) { int i;
  char s[512];
  double v,v1, v2;
  v1=atof(a1); v2=atof(sv);
  switch(op) {
    case '+' : v=v1+v2; break;
    case '-' : v=v1-v2; break;
    case '*' : v=v1*v2; break;
    case '/' : v=v1/v2; break;
    default: v=0;
  }
  double2string(v,s);
  // sprintf(s,"%lf",v);
  // for(i=strlen(s)-1; i>0 && s[i]=='0'; i--) s[i]=0;
  // if (i>0 && s[i]=='.')s[i]=0;
  return strdup(s);
}

char antiOp(char op) {
  switch(op) {
    case '+': return '-';
    case '-': return '+';
    case '*': return '/';
    case '/': return '*';
  }
  /* Ne seert a rien */
  return '=';
}

int memeCatOp(char op1, char op2) {
  if (op1==op2 || op1==antiOp(op2)) return 1;
  else return 0;
}

char typeOp(char op) {
  switch(op) {
    case '+':case '-': return '+';
    case '*':case '/': return '*';
  }
}

char typeOpBrancheGauche(ARBRE a) {
  return typeOp(a->val[0]);
}

char typeOpBrancheDroite(ARBRE a) {
  return a->val[0]=='+'|| a->val[0]=='*' ? a->val[0] : antiOp(typeOp(a->val[0]));
}

char typeOp2Noeud(char c1, char c2) {
  if(c1==c2) return typeOp(c1);
  else return antiOp(typeOp(c1));
}

/* a est la feuille doirte de 0 - x */
/* Return NULL si pas simplifie , l'arbre simplifie sinon
 si x=a-b, retourne b-a
 si x=a+b, retourne simplMoinsUnaire(a) - b ou simplMoinsUnaire(b) - a
 si x=a*b, retourne simplMoinsUnaire(a) * b ou simplMoinsUnaire(b) * a
 si x=a/b, retourne simplMoinsUnaire(a) / b ou simplMoinsUnaire(b) / a */
ARBRE simplMoinsUnaire(ARBRE a) { ARBRE c;
  if (a==NULL || a->type==VARIABLE) return NULL;
  if (a->type==VALEUR) { char s[512];
    double2string(-1*atof(a->val),s);
    free(a->val); a->val=strdup(s);
    return a;
  }
  if (a->type==OPERATEUR_BINAIRE){
    switch (a->val[0]) {
      case '-': c=a->fg; a->fg=a->fd; a->fd=c; return a;
      case '+':
        if ( (c=simplMoinsUnaire(a->fg))!=NULL) { a->fg=c; a->val[0]='-';  return a;}
        else if ( (c=simplMoinsUnaire(a->fd))!=NULL) { a->fd=a->fg; a->fg=c; a->val[0]='-'; return a;}
            else return NULL;
      case '*': case '/':
        if ( (c=simplMoinsUnaire(a->fg))!=NULL) { a->fg=c;  return a;}
        else if ( (c=simplMoinsUnaire(a->fd))!=NULL) { a->fd=c;  return a;}
          else return NULL;
      default: return NULL;
    }
  }
  if (a->type==OPERATEUR_UNAIRE )  {
    if (a->val[0]=='-') { ARBRE b=a->fd; libereNoeud(a); return b; }
    else
      //if ( (c=simplMoinsUnaire(a->fd))!=NULL) { a->fd=c;  return a;}
      //else
        return NULL;
    }
  return NULL;
}

/* retournze l'arbre modife avec ajout de r (qui est une constnnat) dans une constante de l'arbre, NULL si on ne peut ajoute v a l'arbre
A est l'arbre dnas lequel on ajoute
r est le chaine contenant la constnante a ajouter
typea = '+' poour une addition qui se proepage '-' pour une soustraction : arbre a addirif ou multiplicatif
typer = idem, mais pour le neoud r
Exemple :
ajout de 5 a  + 2 + x x retourne + 7 + x x
ajout de 5 a  + x + x x retourne NULL
*/

ARBRE ajouteSousArbre(ARBRE a, char* r, char  typea, char typer) {
  char type;
  if (a==NULL || a->type==VARIABLE) return NULL;
  if (a->type==VALEUR ) { type = typeOp2Noeud(typea,typer);
    char* tmp=a->val;
    type = typeOp2Noeud(typea,typer);
    a->val=opNoeud(r,a->val,type);
    free(tmp);
    return a;
  }
  if (a->type==OPERATEUR_BINAIRE && a->val[0]!='^') {ARBRE c1,c2,c; c=c1=c2=NULL;
    switch (a->val[0]) {
      case '+': case '-': case'*': case '/':
      type = typeOp2Noeud(typea,typer);
      /* cas d'un arbre avec un noued + dans un arbre multiplication ou 'inverse */
        if (  ((a->val[0]=='+'||a->val[0]=='-')&& (type!='+' && type!='-')) ||
              ((a->val[0]=='*'||a->val[0]=='/')&& (type!='*' && type!='/')) )
                {  return NULL;}
                /* le neoud est une valeur, on fai tl'operation et on retrouen */
          if (a->fg->type==VALEUR){ type = typeOp2Noeud(typea,typer);
            char* tmp=a->fg->val;
            a->fg->val=typea=='-'||typea=='/' ? opNoeud(a->fg->val,r,type) : opNoeud(a->fg->val,r,type);
            free(tmp);
            return a;
          }
          else if (a->fd->type==VALEUR){ type = typeOp2Noeud(typea,typer);
            char* tmp=a->fd->val;
            char op;
            if (a->val[0]==type) op=(type=='+'||type=='-')?'+':'*';
            else op=(type=='+'||type=='-')?'-':'/';
            a->fd->val=opNoeud(a->fd->val,r, op);
            free(tmp);
            return a;
          }
          else { char op; type = typeOp2Noeud(typea,typer);
            if (a->val[0]==type) op=(type=='+'||type=='-')?'+':'*';
            else op=(type=='+'||type=='-')?'-':'/';
            if ( (c= ajouteSousArbre(a->fg,r,typea,typer))!=NULL ||
              (c= ajouteSousArbre(a->fd,r,op,typer))!=NULL) {
               return a;
            }
            else return NULL;
          }
      }
    }
    return NULL;
  }

/* Recherche un noued portant une constante et indique s'i lest dnas une branche +,-,* ou/ */
ARBRE rechCte(ARBRE a,char* ptype) {
  if (a==NULL)return a;
  else {
    if (a->type==OPERATEUR_BINAIRE)
      if (memeCatOp(*ptype,a->val[0])) {
        ARBRE c=rechCte(a->fg,ptype);
        if (c)return c;
        else { if (a->val[0]=='-'||a->val[0]=='/') *ptype= antiOp(*ptype);
          return rechCte(a->fd,ptype);
        }
      }
      else return NULL;
    else
      if (a->type==OPERATEUR_UNAIRE && a->val[0]=='-') {
          *ptype= antiOp(*ptype);
          return rechCte(a->fd,ptype);
      }
      else
        if(a->type==VALEUR) return a;
        else return NULL;
  }
}

/*
  Recherche les constantes dans les sous branches pour remonter les operations entre constantes
  dans un arbre.
  Pour chaque constante trouvee, cherche une autre constante de meme type (additive ou multiplicative)
  grace a la fonction ajouteSousArbre
  */
ARBRE simpl2(ARBRE a) { ARBRE c,c1,c2,c3;
  if (a==NULL) return NULL;
  if (a->type==OPERATEUR_UNAIRE) {
    a->fd=simpl2(a->fd); return a;
  }
  if (a->type==VALEUR || a->type==VARIABLE ) return a;
  a->fg=simpl2(a->fg);
  a->fd=simpl2(a->fd);

  if (a->type==OPERATEUR_BINAIRE && a->val[0]!='^') {
    if (a->fg->type==VALEUR){  c1=a->fd; c2=a->fg; c=ajouteSousArbre(c1,c2->val,a->val[0],typeOp(a->val[0])); }
    else if (a->fd->type==VALEUR){  c1=a->fg; c2=a->fd; c=ajouteSousArbre(c1,c2->val,typeOp(a->val[0]),a->val[0]);}
    else {c1=c2=NULL;}
    if (c1 || c2) {
      if (c==NULL) return a;
      else { /* on a trouve une constante dans a et dnas le sousarbre a->fg ou a->fd et fait l'operation
               cette constante et a; le resultat est dnas la constnante, donc on supprime a indirectement
               en mettant 0 pour une addition et 1 pour une multiplication qui seront supprimes
               par la prochaine Simplification
               */
          free(c2->val);
          if (a->val[0]=='+' || a->val[0]=='-') { c2->val=strdup("0");}
          else {  c2->val=strdup("1"); }
          return a;
      }
    }
    else { char typebranche=typeOp(a->val[0]);
      c3=rechCte(a->fg,&typebranche);
      if (c3) { c=ajouteSousArbre(a->fd,c3->val,a->val[0],typebranche);
        if (c!=NULL) {
          free(c3->val);
          if (a->val[0]=='+' || a->val[0]=='-') { c3->val=strdup("0");}
          else { c3->val=strdup("1");}
          a->fd=simpl2(a->fd);
        }
      }
      typebranche=typeOp(a->val[0]);
      c3=rechCte(a->fd,&typebranche);
      if (c3) { c=ajouteSousArbre(a->fg,c3->val,a->val[0],typebranche);
        if (c!=NULL) {
          free(c3->val);
          if (a->val[0]=='+' || a->val[0]=='-') { c3->val=strdup("0");}
          else { c3->val=strdup("1"); }
          a->fg=simpl2(a->fg);
          return (a);
        }
      }
      else return a;
    }
  }
  return a;
}



#define SWAP(x,y,type) { type inter; inter=(x); (x)=(y); (y)=inter; }

ARBRE simplifie1(ARBRE r) {
    //r = distribution(r);
    if (r!=NULL) {
        switch(r->type) {
            case VALEUR: case VARIABLE : return r;
            case OPERATEUR_UNAIRE:
              r->fd=simplifie1(r->fd);
              if (r->fd && r->fd->type==VALEUR) {char s[512];
                double2string(evalArbre(r,0),s);
                libereArbre(r);
                return creerNoeud(s);
              }
              return r;
            case OPERATEUR_BINAIRE :
              r->fg=simplifie1(r->fg);
              r->fd=simplifie1(r->fd);

                  /* Cas de constante operator constante */
                if (r->fd && r->fd->type==VALEUR && r->fg && r->fg->type==VALEUR) { char s[512];
                    double2string(evalArbre(r,0),s);
                    libereArbre(r);
                    return creerNoeud(s);
                }
                if(r->val[0]=='+') { ARBRE cfd=r->fd, cfg=r->fg, c=NULL;
                      /* Cas du x+0 ou 0+x */
                    if((cfd->type==VALEUR && atof(cfd->val)==0 && (c=cfd)!=NULL) || (cfg->type==VALEUR && atof(cfg->val)==0&& (c=cfg)!=NULL)){
                        libereNoeud(r); libereNoeud(c);
                        return simplifie1(c==cfg? cfd : cfg);
                    } /* Cas du x + -y unaire ==> x - y*/
		                if (cfd->type==OPERATEUR_UNAIRE && cfd->val[0]=='-') {
		                  r->val[0]='-'; r->fd=cfd->fd; libereNoeud(cfd); return r;
		                } /* Cas du -unaire x + y ==> y - x */
    		            if (cfg->type==OPERATEUR_UNAIRE && cfg->val[0]=='-') {
		                  r->val[0]='-'; r->fg=cfd; r->fd=cfg->fd; libereNoeud(cfg); return r;
		                }
                      /* cas de + - b a a ==> b */
                    if (cfg->type==OPERATEUR_BINAIRE && cfg->val[0]=='-' && egalArbre(cfg->fd,cfd)) { ARBRE s=NULL;
                        s=cfg->fg; libereArbre(cfg->fd); libereNoeud(cfg); libereArbre(cfd); libereNoeud(r); return s;
                    }
                    /* cas de + a - b a  ==> b */
                    if (cfd->type==OPERATEUR_BINAIRE && cfd->val[0]=='-' && egalArbre(cfd->fd,cfg)) {ARBRE s=NULL;
                        s=cfd->fg; libereArbre(cfd->fd); libereNoeud(cfd); libereArbre(cfg); libereNoeud(r); return s;
                    }

                    if(cfd->type==VALEUR || cfg->type==VALEUR)  return r;
                    else  return (r);
                }
                else /* Cas du x - 0*/
                  if(r->val[0]=='-') { ARBRE cfd=r->fd, cfg=r->fg, c=NULL;
                    if(cfd->type==VALEUR && atof(cfd->val)==0) { libereNoeud(r); libereNoeud(cfd); return simplifie1(cfg); }
                    else { /* Cas du expression - expression */
                      if (egalArbre(r->fd,r->fg)) {
                        libereArbre(r); return creerNoeud("0");
                      } /* Cas du 0 -x */
                      else  /* On cherche si on peu tsuppirmer le - par transformation de 0 -(a-b) en (b-a) et autres*/
		                    if (cfg->type==VALEUR && atof(cfg->val)==0) {
		                      if ( (c=simplMoinsUnaire(r->fd))!=NULL) {
                            libereNoeud(r->fg); libereNoeud(r);
                            return c;
                          }
                          else  /* cas du 0 - Variable ==> -unaire Variable */
			                      if (cfd->type==VARIABLE) {
			                           r->fg=libereNoeud(r->fg); r->type=OPERATEUR_UNAIRE;
			                           return r;
			                       }
                             else return r;
                        }
                        else { /* cas du x - -unairey ==> x + y */
                                if (cfd->type==OPERATEUR_UNAIRE && cfd->val[0]=='-') {
                                    r->val[0]='+'; r->fd=cfd->fd; libereNoeud(cfd); return r;
                                }
                                if (cfg->type==OPERATEUR_BINAIRE && cfg->val[0]=='+') {
                                    /* cas de - + a b a ==> b */
                                  if (egalArbre(cfg->fg,cfd)) { ARBRE s=NULL;
                                    s=cfg->fd; libereNoeud(r); libereArbre(cfg->fg); libereNoeud(cfg); libereArbre(cfd); return s;
                                  }
                                  else /* cas de - + b a a ==> b */
                                    if (egalArbre(cfg->fd,cfd)) {ARBRE s=NULL;
                                      s=cfg->fg; libereNoeud(r);libereArbre(cfg->fd); libereNoeud(cfg); libereArbre(cfd); return s;
                                    }
                                }
                                else
                                  if (cfd->type==OPERATEUR_BINAIRE && cfd->val[0]=='-') {
                                    /* cas de - a - a b ==>  b */
                                    if (egalArbre(cfg,cfd->fg)) {ARBRE s=NULL;
                                      s=cfd->fd; libereNoeud(r);libereArbre(cfd->fg); libereNoeud(cfd); libereArbre(cfg); return s;
                                    }
                                  }
                                return (r);
                        }
                        //else return (r);
                    }
                }
                else if (r->val[0]=='*') { ARBRE cfd=r->fd, cfg=r->fg;
                    if(cfd->type==VALEUR && atof(cfd->val)==0) { libereArbre(r); return creerNoeud("0"); }
                    if(cfg->type==VALEUR && atof(cfg->val)==0) { libereArbre(r); return creerNoeud("0"); }
                    if(cfd->type==VALEUR && atof(cfd->val)==1) { libereNoeud(r); libereNoeud(cfd); return simplifie1(cfg); }
                    if(cfg->type==VALEUR && atof(cfg->val)==1) { libereNoeud(r); libereNoeud(cfg); return simplifie1(cfd); }
                    /* cas de * / b a a ==> b */
                    if (cfg->type==OPERATEUR_BINAIRE && cfg->val[0]=='/' && egalArbre(cfg->fd,cfd)) { ARBRE s=NULL;
                      s=cfg->fg; libereArbre(cfg->fd); libereNoeud(cfg); libereArbre(cfd); libereNoeud(r); return s;
                    }
                    /* cas de * a / b a  ==> b */
                    if (cfd->type==OPERATEUR_BINAIRE && cfd->val[0]=='/' && egalArbre(cfd->fd,cfg)) {ARBRE s=NULL;
                      s=cfd->fg; libereArbre(cfd->fd); libereNoeud(cfd); libereArbre(cfg); libereNoeud(r); return s;
                    }
                    return (r);
                }
                else
                  if (r->val[0]=='/') { ARBRE cfd=r->fd, cfg=r->fg, c=NULL;
                    if(cfg->type==VALEUR) {
                      if (atof(cfg->val)==0) {libereArbre(r); return creerNoeud("0");}
                      /* cas du 1/ (x/y) */
                      if (atof(cfg->val)==1 && cfd->type==OPERATEUR_BINAIRE && cfd->val[0]=='/') {
                        libereNoeud(cfg); libereNoeud(r); c=cfd->fg; cfd->fg=cfd->fd; cfd->fd=c;
                        return cfd;
                      }
                      else return r;
                    }
                    else /* Cas de x / 1 */
                      if(cfd->type==VALEUR && atof(cfd->val)==1) { free(r->val); free(r);; free(cfd->val); free(cfd); return simplifie1(cfg); }
                      else   {/* Cas du expr / expr */
                        if (egalArbre(r->fd,r->fg)) {
                          libereArbre(r); return creerNoeud("1");
                        }
                        /* cas de / * a b a ==> b */
                        if (cfg->type==OPERATEUR_BINAIRE && cfg->val[0]=='*')
                          if (egalArbre(cfg->fg,cfd)) { ARBRE s=NULL;
                            s=cfg->fd; libereArbre(cfg->fg); libereNoeud(cfg); libereArbre(cfd); libereNoeud(r); return s;
                          }
                          else /* cas de / * b a a ==> b */
                            if (egalArbre(cfg->fd,cfd)) { ARBRE s=NULL;
                              s=cfg->fg; libereArbre(cfg->fd); libereNoeud(cfg); libereArbre(cfd); libereNoeud(r); return s;
                            }
                        return r;
                      }
                    return r;
                  }
                  else if (r->val[0]=='^') { ARBRE cfd=r->fd, cfg=r->fg;
                    if(cfd->type==VALEUR && atof(cfd->val)==1) { libereNoeud(r);; libereNoeud(cfd); return simplifie1(cfg); }
                    if(cfd->type==VALEUR && atof(cfd->val)==0) { libereArbre(r); return(creerNoeud("1")); }
                    return (r);
                }
                else { return r; }
        }
    }
    return r;
}

ARBRE  simplifieArbre(ARBRE r) { ARBRE r1=NULL;
    do {
        libereArbre(r1);
        r1=copieArbre(r);
        r=simplifie1(r);
        r=simpl2(r);
//        r=distribution(r);
    } while (!egalArbre(r1,r));
    libereArbre(r1);
    return (r);
}


ARBRE distribution( ARBRE r) { ARBRE f3;
    if (r!=NULL) {
        switch(r->type) {
            case VALEUR: case VARIABLE : case OPERATEUR_UNAIRE: return r;
            case OPERATEUR_BINAIRE :
	           if(r->val[0]=='*' && r->fg->val[0]=='+') {
	              r->val[0]='+'; r->fg->val[0]='*'; f3=r->fd; r->fd=creerNoeud("*");
		            r->fd->fg= r->fg->fd;  r->fg->fd=copieArbre(f3); r->fd->fd=f3;
		            return distribution(r);
	           }
	           else
	            if(r->val[0]=='*' && r->fd->val[0]=='+') {
                 r->val[0]='+'; r->fd->val[0]='*'; f3=r->fg; r->fg=creerNoeud("*");
                 r->fg->fd= r->fd->fg;  r->fd->fg=copieArbre(f3); r->fg->fg=f3;
                 return distribution(r);
	       }
	       else {
	         r->fg = distribution(r->fg);
	         r->fd = distribution(r->fd);
		 return r;
	       }
       }
    }
  return r;
}
