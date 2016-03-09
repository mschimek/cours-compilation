#include <stdio.h>
#include <stdlib.h>
#include "analyseur_lexical.h"
#include "analyseur_syntaxique.h"
#include "analyseur_syntaxique_LL.h"
#include "symboles.h"
#include "util.h"
#include "premiers.h"
#include "suivants.h"
#define LENGTH 100
char nom[LENGTH];
char valeur[LENGTH];
char yytext[100];
FILE *yyin;
int uniteCourante;

void effacer(void) {
	for (int i = 0; i < LENGTH; i++) {
		nom[i] = '\0';
		valeur[i] = '\0';
	}
}
void afficher(void) {
	effacer();
	nom_token(uniteCourante,nom,valeur);
	affiche_element(nom,valeur, TRACE_XML);
}
void szntqx8error(void)
{
	fprintf(stderr, "Erreur de syntaxe\n");
	exit(1);
}

void programme(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (est_premier(_optDecVariables_, uniteCourante)
	    || est_premier(_listeDecFonctions_, uniteCourante)
		|| est_suivant(_programme_, uniteCourante)) {
		optDecVariables();
		listeDecFonctions();
		goto end;
	}

	szntqx8error();

end:
	affiche_balise_fermante(__func__, TRACE_XML);
}

void optDecVariables(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (est_premier(_listeDecVariables_, uniteCourante)) {
		listeDecVariables();
		if (uniteCourante == POINT_VIRGULE) {
			afficher();
			uniteCourante = yylex();
			goto end;
		} 
	} 
	else if (est_suivant(_optDecVariables_, uniteCourante))
			goto end;
	
	szntqx8error();
	
end: 
	affiche_balise_fermante(__func__, TRACE_XML);
}

void listeDecVariables(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (est_premier(_declarationVariable_, uniteCourante)) {
		declarationVariable();
		listeDecVariablesBis();
		goto end;
	}
	
	szntqx8error();
	
end:
	affiche_balise_fermante(__func__, TRACE_XML);
}

void listeDecVariablesBis(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == VIRGULE) {
		afficher();
		uniteCourante = yylex();
		declarationVariable();
		listeDecVariablesBis();
		goto end;
	}
	else if (est_suivant(_listeDecVariablesBis_, uniteCourante))
		goto end;
	
	szntqx8error();

end:
	affiche_balise_fermante(__func__, TRACE_XML);
}
	

void declarationVariable(void) 
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante  == ENTIER) {
		afficher();
		uniteCourante = yylex();
		if (uniteCourante == ID_VAR) {
			afficher();
			uniteCourante = yylex();
			optTailleTableau();
			goto end;
		}
	}
	
	szntqx8error();
	
end:
	affiche_balise_fermante(__func__, uniteCourante);
}
void optTailleTableau(void) 
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == CROCHET_OUVRANT) {
		afficher();
		uniteCourante = yylex();
		if (uniteCourante == NOMBRE) {
			afficher();
			uniteCourante = yylex();
			if (uniteCourante == CROCHET_FERMANT) {
				afficher();
				uniteCourante = yylex();
				goto end;
			}
		}
	}
	else if (est_suivant(_optTailleTableau_, uniteCourante))
		goto end;

	szntqx8error();
	
end:
	affiche_balise_fermante(__func__, TRACE_XML);
}
void listeDecFonctions(void) 
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (est_premier(_declarationFonction_, uniteCourante)) {
		declarationFonction();
		listeDecFonctions();
		goto end;
	}
	else if (est_suivant(_listeDecFonctions_, uniteCourante)) {
		goto end;
	}
	
	szntqx8error();
	
end:
	affiche_balise_fermante(__func__, TRACE_XML);
}
void declarationFonction(void) 
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == ID_FCT) {
		afficher();
		uniteCourante = yylex();
		listeParam();
		optDecVariables();
		instructionBloc();
		goto end;
	}
	
	szntqx8error();
	
end:
	affiche_balise_fermante(__func__, TRACE_XML);
}
void listeParam(void) 
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == PARENTHESE_OUVRANTE) {
		afficher();
		uniteCourante = yylex();
		optListeDecVariables();
		if (uniteCourante == PARENTHESE_FERMANTE) {
			afficher();
			uniteCourante = yylex();
			goto end;
		}
	}
	
	szntqx8error();
	
end:
	affiche_balise_fermante(__func__, TRACE_XML);
}
void optListeDecVariables(void) 
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (est_premier(_listeDecVariables_, uniteCourante)) {
		listeDecVariables();
		goto end;
	}
	else if (est_suivant(_optListeDecVariables_, uniteCourante)) 
		goto end;
	
	szntqx8error();
	
end: 
	affiche_balise_fermante(__func__, TRACE_XML);
}
void instruction(void) 
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (est_premier(_instructionAffect_, uniteCourante)) {
		instructionAffect();
		goto end;
	}
	else if (est_premier(_instructionBloc_, uniteCourante)) {
		instructionBloc();
		goto end;
	}
	else if (est_premier(_instructionSi_, uniteCourante)) {
		instructionSi();
		goto end;
	}
	else if (est_premier(_instructionTantque_, uniteCourante)) {
		instructionTantque();
		goto end;
	}
	else if (est_premier(_instructionAppel_, uniteCourante)) {
		instructionAppel();
		goto end;
	}
	else if (est_premier(_instructionRetour_, uniteCourante)) {
		instructionRetour();
		goto end;
	}
	else if (est_premier(_instructionEcriture_, uniteCourante)) {
		instructionEcriture();
		goto end;
	}
	else if (est_premier(_instructionVide_, uniteCourante)) {
		instructionVide();
		goto end;
	}
	
	szntqx8error();
	
end:
	affiche_balise_fermante(__func__, TRACE_XML);
}
void instructionAffect(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (est_premier(_var_, uniteCourante)) {
		var();
		if (uniteCourante == EGAL) {
			afficher();
			uniteCourante = yylex();
			if (est_premier(_expression_, uniteCourante)) {
				expression();
				if (uniteCourante == POINT_VIRGULE) {
					afficher();
					uniteCourante = yylex();
					goto end;
				}
			}
		}
	}
	
	szntqx8error();
	
end:
	affiche_balise_fermante(__func__, TRACE_XML);
}
void instructionBloc(void) 
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == ACCOLADE_OUVRANTE) {
		afficher();
		uniteCourante = yylex();
		listeInstructions();
		if (uniteCourante == ACCOLADE_FERMANTE) {
			afficher();
			uniteCourante = yylex();
			goto end;
		}
	}
	
	szntqx8error();
	
end: 
	affiche_balise_fermante(__func__, TRACE_XML);
	
}
void listeInstructions(void) 
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (est_premier(_instruction_, uniteCourante)) {
		instruction();
		listeInstructions();
		goto end;
	}
	else if (est_suivant(_listeInstructions_, uniteCourante))
		goto end;
	
	szntqx8error();
	
end:
	affiche_balise_fermante(__func__, TRACE_XML);
}
void instructionSi(void) 
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == SI) {
		afficher();
		uniteCourante = yylex();
		expression();
		if (uniteCourante == ALORS) {
			afficher();
			uniteCourante = yylex();
			instructionBloc();
			optSinon();
			goto end;
		}
	}
	
	szntqx8error();
	
end: 
	affiche_balise_fermante(__func__, TRACE_XML);
}

void optSinon(void) 
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == SINON) {
		afficher();
		uniteCourante = yylex();
		instructionBloc();
		goto end;
	}
	else if (est_suivant(_optSinon_, uniteCourante))
		goto end;
	
	szntqx8error();
	
end:
	affiche_balise_fermante(__func__, TRACE_XML);
} 
void instructionTantque(void) 
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == TANTQUE) {
		afficher();
		uniteCourante = yylex();
		expression();
		if (uniteCourante == FAIRE) {
			afficher();
			uniteCourante = yylex();
			instructionBloc();
			goto end;
		}
	}
	
	szntqx8error();
	
end: 
	affiche_balise_fermante(__func__, TRACE_XML);
}
void instructionAppel(void) 
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (est_premier(_appelFct_, uniteCourante)) {
		appelFct();
		if (uniteCourante == POINT_VIRGULE) {
			afficher();
			uniteCourante = yylex();
			goto end;
		}
	}
	
	szntqx8error();
	
end:
	affiche_balise_fermante(__func__, TRACE_XML);
}
void instructionRetour(void) 
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == RETOUR) {
		afficher();
		uniteCourante = yylex();
		expression();
		if (uniteCourante == POINT_VIRGULE) {
			afficher();
			uniteCourante = yylex();
			goto end;
		}
	}
	
	szntqx8error();
	
end:
	affiche_balise_fermante(__func__, TRACE_XML);
}
void instructionEcriture(void) 
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == ECRIRE) {
		afficher();
		uniteCourante = yylex();
		if (uniteCourante == PARENTHESE_OUVRANTE) {
			afficher();
			uniteCourante = yylex();
			expression();
			if (uniteCourante == PARENTHESE_FERMANTE) {
				afficher();
				uniteCourante = yylex();
				if (uniteCourante == POINT_VIRGULE) {
					afficher();
					uniteCourante = yylex();
					goto end;
				}
			}
		}
	}
	
	szntqx8error();

end:
	affiche_balise_fermante(__func__, TRACE_XML);
}
void instructionVide(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == POINT_VIRGULE) {
		afficher();
		uniteCourante = yylex();
		goto end;
	}
	
	szntqx8error();
	
end: 
	affiche_balise_fermante(__func__, TRACE_XML);
}
void expression(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (est_premier(_conjonction_, uniteCourante)) {
		conjonction();
		expressionBis();
		goto end;
	}
	
	szntqx8error();
	
end:
	affiche_balise_fermante(__func__, TRACE_XML);
}

void expressionBis(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == OU) {
		afficher();
		uniteCourante = yylex();
		conjonction();
		expressionBis();
		goto end;
	}
	else if (est_suivant(_expressionBis_, uniteCourante))
		goto end;
	
	szntqx8error();

end:
	affiche_balise_fermante(__func__, TRACE_XML);
}

void conjonction(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (est_premier(_negation_, uniteCourante)) {
		negation();
		conjonctionBis();
		goto end;
	}
	
	szntqx8error();
	
end:
	affiche_balise_fermante(__func__, TRACE_XML);
}

void conjonctionBis(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == ET) {
		afficher();
		uniteCourante = yylex();
		negation();
		conjonctionBis();
		goto end;
	}
	else if (est_suivant(_conjonctionBis_, uniteCourante))
		goto end;
	
	szntqx8error();

end:
	affiche_balise_fermante(__func__, TRACE_XML);
}

void negation(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == NON) {
		afficher();
		uniteCourante = yylex();
		comparaison();
		goto end;
	}
	else if (est_premier(_comparaison_, uniteCourante)) {
		comparaison();
		goto end;
	}
	szntqx8error();
	
end:
	affiche_balise_fermante(__func__, TRACE_XML);
}

void comparaison(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (est_premier(_comparaison_, uniteCourante)) {
		expArith();
		comparaisonBis();
		goto end;
	}
	szntqx8error();

end:	
	affiche_balise_fermante(__func__, TRACE_XML);
}

void comparaisonBis(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == EGAL || uniteCourante == INFERIEUR) {
		afficher();
		uniteCourante = yylex();
		expArith();
		comparaisonBis();
		goto end;
	}
	else if (est_suivant(_comparaisonBis_, uniteCourante))
		goto end;
	
	szntqx8error();

end:
	affiche_balise_fermante(__func__, TRACE_XML);
}

void expArith(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (est_premier(_terme_, uniteCourante)) {
		terme();
		expArithBis();
		goto end;
	}
	
	szntqx8error();

end:	
	affiche_balise_fermante(__func__, TRACE_XML);
}

void expArithBis(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == PLUS || uniteCourante == MOINS) {
		afficher();
		uniteCourante = yylex();
		terme();
		expArithBis();
		goto end;
	}
	else if (est_suivant(_expArithBis_, uniteCourante))
		goto end;
	
	szntqx8error();

end:
	affiche_balise_fermante(__func__, TRACE_XML);
}

void terme(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (est_premier(_facteur_, uniteCourante)) {
		facteur();
		termeBis();
		goto end;
	}
	szntqx8error();

end: 
	affiche_balise_fermante(__func__, TRACE_XML);
}

void termeBis(void) {
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == FOIS || uniteCourante == DIVISE) {
		afficher();
		uniteCourante = yylex();
		facteur();
		termeBis();
		goto end;
	}
	else if (est_suivant(_termeBis_, uniteCourante))
		goto end;
	
	szntqx8error();
	
end: 
	affiche_balise_fermante(__func__, TRACE_XML);
}

void facteur(void) {
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == PARENTHESE_OUVRANTE) {
		afficher();
		uniteCourante = yylex();
		expression();
		if (uniteCourante == PARENTHESE_FERMANTE) {
			afficher();
			uniteCourante = yylex();
			goto end;
		}
			
	}
	else if (uniteCourante == NOMBRE) {
		afficher();
		uniteCourante = yylex();
		goto end;
	}
	else if (est_premier(_appelFct_, uniteCourante)) {
		appelFct();
		goto end;
	}
	else if (est_premier(_var_, uniteCourante)) {
		var();
		goto end;
	}
	else if (uniteCourante == LIRE) {
		afficher();
		uniteCourante = yylex();
		if (uniteCourante == PARENTHESE_OUVRANTE) {
			afficher();
			uniteCourante = yylex();
			if (uniteCourante == PARENTHESE_FERMANTE)
				afficher();
				uniteCourante = yylex();
				goto end;
		}
	}
	szntqx8error();

end:
	affiche_balise_fermante(__func__, TRACE_XML);


}


void var(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == ID_VAR) {
		afficher();
		uniteCourante = yylex();
		optIndice();
		goto end;
	}
	else
		szntqx8error();
end:
	affiche_balise_fermante(__func__, TRACE_XML);
	
}
void optIndice(void)
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == CROCHET_OUVRANT) {
		afficher();
		uniteCourante = yylex();
		expression();
		if (uniteCourante == CROCHET_FERMANT) {
			afficher();
			uniteCourante = yylex();
			goto end;
		}
	}
	else if (est_suivant(_optIndice_, uniteCourante))
		goto end;
	
	szntqx8error();
	
end:
	affiche_balise_fermante(__func__, TRACE_XML);
}
void appelFct(void) 
{
	affiche_balise_ouvrante(__func__, TRACE_XML); 
	if (uniteCourante == ID_FCT) {
		afficher();
		uniteCourante = yylex();
		if (uniteCourante == PARENTHESE_OUVRANTE) {
			afficher();
			uniteCourante = yylex();
			listeExpressions();
			if (uniteCourante == PARENTHESE_FERMANTE) {
				afficher();
				uniteCourante = yylex();
				goto end;
			}
				
			else 
				szntqx8error();
		}
		else
			szntqx8error();
	}
	else
		szntqx8error();

end: 
	affiche_balise_fermante(__func__, TRACE_XML);
		
}
void listeExpressions(void) 
{
	
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (est_premier(_expression_, uniteCourante)){
		expression();
		listeExpressionsBis();
		goto end;
	}
	else if (est_suivant(_listeExpressions_, uniteCourante)) 
		goto end;
	else 
		szntqx8error();
end:
	affiche_balise_fermante(__func__, TRACE_XML);
}

void listeExpressionsBis(void) 
{
	affiche_balise_ouvrante(__func__, TRACE_XML);
	if (uniteCourante == VIRGULE) {
		afficher();
		uniteCourante = yylex();
		expression();
		listeExpressionsBis();
		goto end;
	}
	else if (est_suivant(_listeExpressionsBis_,uniteCourante)) {
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
  programme();
  if (uniteCourante != FIN) {
	szntqx8error();
  }
  return 0;
}

