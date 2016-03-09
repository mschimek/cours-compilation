#include <stdio.h>
#include <stdlib.h>
#include "symboles.h"
#include "util.h"

#define TRACE_XML 1

char yytext[100];
FILE *yyin;
int uniteCourante;

void szntqx8error(void)
{
	fprintf(stderr, "Erreur de syntaxe\n");
	exit(1);
}

void expression(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	affiche_balise_fermante(__func__, TRACE_XML);
}

	
int main(int argc, char **argv) {  
  yyin = fopen(argv[1], "r");
  if(yyin == NULL){
    fprintf(stderr, "impossible d'ouvrir le fichier %s\n", argv[1]);
    exit(1);
  }
  if (uniteCourante != FIN) {
	szntqx8error();
  }
  return 0;
}


