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

void expression(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	conjonction();
	expressionBis();
	affiche_balise_fermante(__func__, TRACE_XML);
}

void expressionBis(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == OU) {
		uniteCourante = yylex();
		conjonction();
		expressionBis();
	}
	affiche_balise_fermante(__func__, TRACE_XML);
}

void conjonction(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	negation();
	conjonctionBis();
	affiche_balise_fermante(__func__, TRACE_XML);
}

void conjonctionBis(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == ET) {
		uniteCourante = yylex();
		negation();
		conjonctionBis();
	}
	affiche_balise_fermante(__func__, TRACE_XML);
}

void negation(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == NON) {
		uniteCourante = yylex();
		comparaison();
	} else
		comparaison();
	affiche_balise_fermante(__func__, TRACE_XML);
}

void comparaison(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	expArith();
	comparaisonBis();
	affiche_balise_fermante(__func__, TRACE_XML);
}

void comparaisonBis(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == EGAL || uniteCourante == INFERIEUR) {
		uniteCourante = yylex();
		expArith();
		comparaisonBis();
	}
	affiche_balise_fermante(__func__, TRACE_XML);
}

void expArith(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	terme();
	expArithBis();
	affiche_balise_fermante(__func__, TRACE_XML);
}

void expArithBis(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == PLUS || uniteCourante == MOINS) {
		uniteCourante = yylex();
		terme();
		expArithBis();
	}
	affiche_balise_fermante(__func__, TRACE_XML);
}

void terme(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	facteur();
	termeBis();
	affiche_balise_fermante(__func__, TRACE_XML);
}

void termeBis(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == FOIS || uniteCourante == DIVISE) {
		uniteCourante = yylex();
		facteur();
		termeBis();
	}
	affiche_balise_fermante(__func__, TRACE_XML);
}

void facteur(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == PARENTHESE_OUVRANTE) {
		uniteCourante = yylex();
		expression();
		if (uniteCourante == PARENTHESE_FERMANTE) {
			uniteCourante = yylex();
			goto end;
		} else
			szntqx8error();
	} else if (uniteCourante == NOMBRE) {
		uniteCourante = yylex();
		goto end;
	#if 0
	} else if (uniteCourante == ID_FCT) { /* TODO : réécrire ça la semaine prochaine */
		appelFct();
	#endif
	} else if (uniteCourante == ID_VAR) {
		var();
	} else /* TODO : LIRE */
		szntqx8error();
end:
	affiche_balise_fermante(__func__, TRACE_XML);
}

void var(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == ID_VAR) {
		uniteCourante = yylex();
		optIndice();
	} else
		szntqx8error();
	affiche_balise_fermante(__func__, TRACE_XML);
}

void optIndice(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == CROCHET_OUVRANT) {
		uniteCourante = yylex();
		expression();
		if (uniteCourante == CROCHET_FERMANT) {
			uniteCourante = yylex();
			goto end;
		} else
			szntqx8error();
	}
end:
	affiche_balise_fermante(__func__, TRACE_XML);
}

#if 0
void appelFct(void)
{
	if (uniteCourante == ID_FCT) {
		uniteCourante = yylex();
		if (uniteCourante == PARENTHESE_OUVRANTE) {
			uniteCourante = yylex();
			listeExpressions();
			if (uniteCourante == PARENTHESE_FERMANTE)
				uniteCourante = yylex();
			else
				szntqx8error();	
		} else
			szntqx8error();
	} else
		szntqx8error();
}

void listeExpressions(void)
{
	expression();
	listeExpressionsBis();

#endif
	
int main(int argc, char **argv) {  
  yyin = fopen(argv[1], "r");
  if(yyin == NULL){
    fprintf(stderr, "impossible d'ouvrir le fichier %s\n", argv[1]);
    exit(1);
  }
  uniteCourante = yylex();
  expression();
  if (uniteCourante != FIN) {
	szntqx8error();
  }
  return 0;
}


