#include <stdio.h>
#include <stdlib.h>
#include "analyseur_lexical.h"
#include "analyseur_syntaxique.h"
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

void E(void) {
	affiche_balise_ouvrante(__func__, TRACE_XML);
	T();
	Eprime();
	affiche_balise_fermante(__func__, TRACE_XML);
}

void Eprime(void) {
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == PLUS) {
		uniteCourante = yylex();
		E();
	}
	affiche_balise_fermante(__func__, TRACE_XML);
}

void T(void) {
	affiche_balise_ouvrante(__func__, TRACE_XML);
	F();
	Tprime();
	affiche_balise_fermante(__func__, TRACE_XML);
}

void Tprime(void) {
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == FOIS) {
		uniteCourante = yylex();
		T();
	}
	affiche_balise_fermante(__func__, TRACE_XML);
}

void F(void) {
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == PARENTHESE_OUVRANTE) {
		uniteCourante = yylex();
		E();
		if (uniteCourante == PARENTHESE_FERMANTE) {
			uniteCourante = yylex();
			goto end;
		} else
			szntqx8error();
	}
	else if (uniteCourante == NOMBRE) {
		uniteCourante = yylex();
		goto end;
	}
	else
		szntqx8error();
end:	
	affiche_balise_fermante(__func__, TRACE_XML);
}




int main(int argc, char **argv) {  
  yyin = fopen(argv[1], "r");
  if(yyin == NULL){
    fprintf(stderr, "impossible d'ouvrir le fichier %s\n", argv[1]);
    exit(1);
  }
  uniteCourante = yylex();
  E();
  if (uniteCourante != FIN) {
	szntqx8error();
  }
  return 0;
}


