#include <stdio.h>
#include <stdlib.h>
#include "analyseur_lexical.h"
#include "symboles.h"
#include "util.h"
#include "premiers.h"
#include "suivants.h"
#include "syntabs.h"
#include "analyseur_tableau.h"
#include "construire_tableau.h"
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

void afficher_sans_afficher(void) {
	effacer();
	nom_token(uniteCourante,nom,valeur);
}



n_prog * programme(void)
{
	n_l_dec *variables, *fonctions;
	
	////affiche_balise_ouvrante__func__, TRACE_XML);
	if (est_premier(_optDecVariables_, uniteCourante)
	    || est_premier(_listeDecFonctions_, uniteCourante)
		|| est_suivant(_programme_, uniteCourante)) {
		variables=optDecVariables();
		fonctions=listeDecFonctions();
		goto end;
	}

	erreur("erreur");

end:
	////affiche_balise_fermante(__func__, TRACE_XML);
	
return cree_n_prog(variables, fonctions);
}

n_l_dec * optDecVariables(void)
{
	n_l_dec * $$ = NULL; 
	////affiche_balise_ouvrante__func__, TRACE_XML);
	if (est_premier(_listeDecVariables_, uniteCourante)) {
		$$ = listeDecVariables();
		if (uniteCourante == POINT_VIRGULE) {
			afficher_sans_afficher();
			uniteCourante = yylex();
			goto end;
		} 
	} 
	else if (est_suivant(_optDecVariables_, uniteCourante))
			goto end;
	
	erreur("");
	
end: 
	////affiche_balise_fermante(__func__, TRACE_XML);
	return $$;
}

n_l_dec * listeDecVariables(void)
{
	n_dec *tete = NULL;
	n_l_dec *queue = NULL;
	n_l_dec * liste = NULL;
	////affiche_balise_ouvrante__func__, TRACE_XML);
	if (est_premier(_declarationVariable_, uniteCourante)) {
		tete = declarationVariable();
		queue = listeDecVariablesBis();
		liste = cree_n_l_dec(tete, queue);
		goto end;
	}
	
	erreur("");
	
end:
	////affiche_balise_fermante(__func__, TRACE_XML);
	return liste;
}

n_l_dec *listeDecVariablesBis(void)
{
	n_dec *tete = NULL;
	n_l_dec *queue = NULL;
	n_l_dec *liste = NULL;
	////affiche_balise_ouvrante__func__, TRACE_XML);
	if (uniteCourante == VIRGULE) {
		afficher_sans_afficher();
		uniteCourante = yylex();
		tete = declarationVariable();
		queue = listeDecVariablesBis();
		liste = cree_n_l_dec(tete, queue);
		goto end;
	}
	else if (est_suivant(_listeDecVariablesBis_, uniteCourante))
		goto end;
	
	erreur("");

end:
	////affiche_balise_fermante(__func__, TRACE_XML);
	return liste;
}
	

n_dec * declarationVariable(void) 
{
	n_dec * $$ = NULL;  
	char * nomVar;
	////affiche_balise_ouvrante__func__, TRACE_XML);
	if (uniteCourante  == ENTIER) {
		afficher_sans_afficher();
		uniteCourante = yylex();
		if (uniteCourante == ID_VAR) {
			afficher_sans_afficher();
			nomVar = duplique_chaine(valeur);
			uniteCourante = yylex();
			$$ = optTailleTableau(nomVar);
			goto end;
		}
	}
	
	erreur("");
	
end:
	//affiche_balise_fermante(__func__, uniteCourante);
	return ($$ == NULL) ? cree_n_dec_var(nomVar) : $$;
}
n_dec *optTailleTableau(char *nom) 
{
	n_dec *$$ = NULL;
	char *taille;
	////affiche_balise_ouvrante__func__, TRACE_XML);
	if (uniteCourante == CROCHET_OUVRANT) {
		afficher_sans_afficher();
		uniteCourante = yylex();
		if (uniteCourante == NOMBRE) {
			afficher_sans_afficher();
			taille = duplique_chaine(valeur);
			uniteCourante = yylex();
			if (uniteCourante == CROCHET_FERMANT) {
				afficher_sans_afficher();
				uniteCourante = yylex();
				$$ = cree_n_dec_tab(nom, atoi(taille));
				goto end;
			}
		}
	}
	else if (est_suivant(_optTailleTableau_, uniteCourante))
		goto end;

	erreur("");
	
end:
	////affiche_balise_fermante(__func__, TRACE_XML);
	return $$;
}
n_l_dec *listeDecFonctions(void) 
{
	n_dec *tete = NULL;
	n_l_dec *queue = NULL;
	n_l_dec * liste = NULL;
	////affiche_balise_ouvrante__func__, TRACE_XML);
	if (est_premier(_declarationFonction_, uniteCourante)) {
		tete = declarationFonction();
		queue = listeDecFonctions();
		liste = cree_n_l_dec(tete, queue);
		goto end;
	}
	else if (est_suivant(_listeDecFonctions_, uniteCourante)) {
		goto end;
	}
	
	erreur("");
	
end:
	////affiche_balise_fermante(__func__, TRACE_XML);
	return liste;
}
n_dec *declarationFonction(void) 
{
	char *nomFonc;
	n_l_dec *param = NULL;
	n_l_dec *variables = NULL;
	n_instr *corps = NULL;
	n_dec * dec = NULL;
	////affiche_balise_ouvrante__func__, TRACE_XML);
	if (uniteCourante == ID_FCT) {
		afficher_sans_afficher();
		nomFonc = duplique_chaine(valeur);
		uniteCourante = yylex();
		param = listeParam();
		variables = optDecVariables();
		corps = instructionBloc();
		dec = cree_n_dec_fonc(nomFonc, param, variables,corps);
		goto end;
	}
	
	erreur("");
	
end:
	////affiche_balise_fermante(__func__, TRACE_XML);
	return dec; 
}
n_l_dec *listeParam(void) 
{
	n_l_dec *$$ = NULL;
	////affiche_balise_ouvrante__func__, TRACE_XML);
	if (uniteCourante == PARENTHESE_OUVRANTE) {
		afficher_sans_afficher();
		uniteCourante = yylex();
		$$ = optListeDecVariables();
		if (uniteCourante == PARENTHESE_FERMANTE) {
			afficher_sans_afficher();
			uniteCourante = yylex();
			goto end;
		}
	}
	
	erreur("");
	
end:
	////affiche_balise_fermante(__func__, TRACE_XML);
	return $$;
}
n_l_dec *optListeDecVariables(void) 
{
	n_l_dec *$$ = NULL;
	//affiche_balise_ouvrante__func__, TRACE_XML);
	if (est_premier(_listeDecVariables_, uniteCourante)) {
		$$ = listeDecVariables();
		goto end;
	}
	else if (est_suivant(_optListeDecVariables_, uniteCourante)) 
		goto end;
	
	erreur("");
	
end: 
	//affiche_balise_fermante(__func__, TRACE_XML);
	return $$;
}
n_instr *instruction(void) 
{
	n_instr *instr = NULL;
	//affiche_balise_ouvrante__func__, TRACE_XML);
	if (est_premier(_instructionAffect_, uniteCourante)) {
		instr = instructionAffect();
		goto end;
	}
	else if (est_premier(_instructionBloc_, uniteCourante)) {
		instr = instructionBloc();
		goto end;
	}
	else if (est_premier(_instructionSi_, uniteCourante)) {
		instr = instructionSi();
		goto end;
	}
	else if (est_premier(_instructionTantque_, uniteCourante)) {
		instr = instructionTantque();
		goto end;
	}
	else if (est_premier(_instructionAppel_, uniteCourante)) {
		instr = instructionAppel();
		goto end;
	}
	else if (est_premier(_instructionRetour_, uniteCourante)) {
		instr = instructionRetour();
		goto end;
	}
	else if (est_premier(_instructionEcriture_, uniteCourante)) {
		instr = instructionEcriture();
		goto end;
	}
	else if (est_premier(_instructionVide_, uniteCourante)) {
		instr = instructionVide();
		goto end;
	}
	
	erreur("");
	
end:
	//affiche_balise_fermante(__func__, TRACE_XML);
	return instr; 
}
n_instr *instructionAffect(void)
{
	n_var *vari = NULL;
	n_exp *exp = NULL;
	n_instr * instr = NULL;
	//affiche_balise_ouvrante__func__, TRACE_XML);
	if (est_premier(_var_, uniteCourante)) {
		vari = var();
		if (uniteCourante == EGAL) {
			afficher_sans_afficher();
			uniteCourante = yylex();
			if (est_premier(_expression_, uniteCourante)) {
				exp = expression();
				if (uniteCourante == POINT_VIRGULE) {
					//afficher_sans_afficher();
					uniteCourante = yylex();
					instr = cree_n_instr_affect(vari, exp);
					goto end;
				}
			}
		}
	}
	
	erreur("");
	
end:
	//affiche_balise_fermante(__func__, TRACE_XML);
	return instr; 
}
n_instr *instructionBloc(void) 
{
	n_l_instr *liste = NULL;
	//affiche_balise_ouvrante__func__, TRACE_XML);
	if (uniteCourante == ACCOLADE_OUVRANTE) {
		//afficher_sans_afficher();
		uniteCourante = yylex();
		liste = listeInstructions();
		if (uniteCourante == ACCOLADE_FERMANTE) {
			//afficher_sans_afficher();
			uniteCourante = yylex();
			goto end;
		}
	}
	
	erreur("");
	
end: 
	//affiche_balise_fermante(__func__, TRACE_XML);
	return cree_n_instr_bloc(liste);
	
}

n_l_instr *listeInstructions(void) 
{
	n_instr *tete = NULL; 
	n_l_instr *queue = NULL;
	n_l_instr *liste = NULL;
	//affiche_balise_ouvrante__func__, TRACE_XML);
	if (est_premier(_instruction_, uniteCourante)) {
		tete =instruction();
		queue = listeInstructions();
		liste = cree_n_l_instr(tete, queue);
		goto end;
	}
	else if (est_suivant(_listeInstructions_, uniteCourante))
		goto end;
	
	erreur("");
	
end:
	//affiche_balise_fermante(__func__, TRACE_XML);
	return liste; 
}
n_instr* instructionSi(void) 
{    
	n_exp * test = NULL;
	n_instr * alors = NULL;
	n_instr * sinon = NULL;
	//affiche_balise_ouvrante__func__, TRACE_XML);
	if (uniteCourante == SI) {
		afficher_sans_afficher();
		uniteCourante = yylex();
		test = expression();
		if (uniteCourante == ALORS) {
			afficher_sans_afficher();
			uniteCourante = yylex();
			alors = instructionBloc();
			sinon = optSinon();
			goto end;
		}
	}
	
	erreur("");
	
end: 
	//affiche_balise_fermante(__func__, TRACE_XML);
	return cree_n_instr_si(test, alors, sinon);
}

n_instr *optSinon(void) 
{
	n_instr *instr = NULL;
	//affiche_balise_ouvrante__func__, TRACE_XML);
	if (uniteCourante == SINON) {
		afficher_sans_afficher();
		uniteCourante = yylex();
		instr = instructionBloc();
		goto end;
	}
	else if (est_suivant(_optSinon_, uniteCourante))
		goto end;
	
	erreur("");
	
end:
	//affiche_balise_fermante(__func__, TRACE_XML);
	return instr;
} 
n_instr *instructionTantque(void) 
{
	n_exp *test;
	n_instr *faire;
	//affiche_balise_ouvrante__func__, TRACE_XML);
	if (uniteCourante == TANTQUE) {
		afficher_sans_afficher();
		uniteCourante = yylex();
		test = expression();
		if (uniteCourante == FAIRE) {
			afficher_sans_afficher();
			uniteCourante = yylex();
			faire = instructionBloc();
			goto end;
		}
	}
	
	erreur("");
	
end: 
	//affiche_balise_fermante(__func__, TRACE_XML);
	return cree_n_instr_tantque(test, faire);
}
n_instr *instructionAppel(void) 
{
	n_appel *appel = NULL;
	//affiche_balise_ouvrante__func__, TRACE_XML);
	if (est_premier(_appelFct_, uniteCourante)) {
		appel = appelFct();
		if (uniteCourante == POINT_VIRGULE) {
			afficher_sans_afficher();
			uniteCourante = yylex();
			goto end;
		}
	}
	
	erreur("");
	
end:
	//affiche_balise_fermante(__func__, TRACE_XML);
	return cree_n_instr_appel(appel);
}
n_instr *instructionRetour(void) 
{
	n_exp *expr = NULL;
	//affiche_balise_ouvrante__func__, TRACE_XML);
	if (uniteCourante == RETOUR) {
		afficher_sans_afficher();
		uniteCourante = yylex();
		expr = expression();
		if (uniteCourante == POINT_VIRGULE) {
			afficher_sans_afficher();
			uniteCourante = yylex();
			goto end;
		}
	}
	
	erreur("");
	
end:
	//affiche_balise_fermante(__func__, TRACE_XML);
	return cree_n_instr_retour(expr);
}
n_instr *instructionEcriture(void) 
{
	n_exp * expr = NULL;
	//affiche_balise_ouvrante__func__, TRACE_XML);
	if (uniteCourante == ECRIRE) {
		afficher_sans_afficher();
		uniteCourante = yylex();
		if (uniteCourante == PARENTHESE_OUVRANTE) {
			afficher_sans_afficher();
			uniteCourante = yylex();
			expr = expression();
			if (uniteCourante == PARENTHESE_FERMANTE) {
				afficher_sans_afficher();
				uniteCourante = yylex();
				if (uniteCourante == POINT_VIRGULE) {
					afficher_sans_afficher();
					uniteCourante = yylex();
					goto end;
				}
			}
		}
	}
	
	erreur("");

end:
	//affiche_balise_fermante(__func__, TRACE_XML);
	return cree_n_instr_ecrire(expr);
}
n_instr *instructionVide(void)
{
	//affiche_balise_ouvrante__func__, TRACE_XML);
	if (uniteCourante == POINT_VIRGULE) {
		afficher_sans_afficher();
		uniteCourante = yylex();
		goto end;
	}
	
	erreur("");
	
end: 
	//affiche_balise_fermante(__func__, TRACE_XML);
	return cree_n_instr_vide();
}
n_exp * expression(void)
{
	n_exp * exp = NULL;
	n_exp * herite = NULL;
	//affiche_balise_ouvrante__func__, TRACE_XML);
	if (est_premier(_conjonction_, uniteCourante)) {
		herite = conjonction();
		exp = expressionBis(herite);
		goto end;
	}
	
	erreur("");
	
end:
	//affiche_balise_fermante(__func__, TRACE_XML);
	return exp;
}

n_exp * expressionBis(n_exp * herite)
{
	n_exp * exp = NULL;
	n_exp * herite_fils = NULL;
	n_exp * res = NULL;
	//affiche_balise_ouvrante__func__, TRACE_XML);
	if (uniteCourante == OU) {
		afficher_sans_afficher();
		uniteCourante = yylex();
		res = conjonction();
		herite_fils = cree_n_exp_op(ou,herite,res);
		exp = expressionBis(herite_fils);
		goto end;
	}
	else if (est_suivant(_expressionBis_, uniteCourante))
		exp = herite;
		goto end;
	
	erreur("");

end:
	//affiche_balise_fermante(__func__, TRACE_XML);
	return exp;
}

n_exp * conjonction(void)
{       
	n_exp * exp = NULL;
	n_exp * res = NULL;
	//affiche_balise_ouvrante__func__, TRACE_XML);
	if (est_premier(_negation_, uniteCourante)) {
		res = negation();
		exp = conjonctionBis(res);
		goto end;
	}
	
	erreur("");
	
end:
	//affiche_balise_fermante(__func__, TRACE_XML);
	return exp;
}

n_exp * conjonctionBis(n_exp * herite)
{
	n_exp * exp = NULL;
	n_exp * res = NULL;
	n_exp * herite_fils = NULL;
	//affiche_balise_ouvrante__func__, TRACE_XML);
	if (uniteCourante == ET) {
		afficher_sans_afficher();
		uniteCourante = yylex();
		res = negation();
		herite_fils = cree_n_exp_op(et,herite,res);
		exp = conjonctionBis(herite_fils);
		goto end;
	}
	else if (est_suivant(_conjonctionBis_, uniteCourante))
		exp = herite;
		goto end;
	
	erreur("");

end:
	//affiche_balise_fermante(__func__, TRACE_XML);
	return exp;
}

/***********************************************************
 * ATTENTION 
 */
n_exp * negation(void)
{
	n_exp * exp = NULL;
	n_exp * res = NULL;
	//affiche_balise_ouvrante__func__, TRACE_XML);
	if (uniteCourante == NON) {
		afficher_sans_afficher();
		uniteCourante = yylex();
		res = comparaison();
		exp = cree_n_exp_op(fois, NULL, res);
		goto end;
	}
	else if (est_premier(_comparaison_, uniteCourante)) {
		exp = comparaison();
		goto end;
	}
	erreur("");
	
end:
	//affiche_balise_fermante(__func__, TRACE_XML);
	return exp;
}

n_exp * comparaison(void)
{
	n_exp * exp = NULL;
	n_exp * res = NULL; 
	//affiche_balise_ouvrante__func__, TRACE_XML);
	if (est_premier(_comparaison_, uniteCourante)) {
		res = expArith();
		exp = comparaisonBis(res);
		goto end;
	}
	erreur("");

end:	
	//affiche_balise_fermante(__func__, TRACE_XML);
	return exp;
}

n_exp * comparaisonBis(n_exp * herite)
{
	n_exp * exp = NULL;
	n_exp * res = NULL;
	n_exp * herite_fils = NULL;
	//affiche_balise_ouvrante__func__, TRACE_XML);
	if (uniteCourante == EGAL) {
		afficher_sans_afficher();
		uniteCourante = yylex();
		res = expArith();
		herite_fils = cree_n_exp_op(egal, herite, res);
		exp = comparaisonBis(herite_fils);
		goto end;
	}
	else if ( uniteCourante == INFERIEUR) {
		afficher_sans_afficher();
		uniteCourante = yylex();
		res = expArith();
		herite_fils = cree_n_exp_op(inf, herite, res);
		exp = comparaisonBis(herite_fils);
		goto end;
	}
	else if (est_suivant(_comparaisonBis_, uniteCourante))
		exp = herite;
		goto end;
	
	erreur("");
 
end:
	//affiche_balise_fermante(__func__, TRACE_XML);
	return exp;
}

n_exp * expArith(void)
{
	n_exp * exp = NULL;
	n_exp * res = NULL;
	//affiche_balise_ouvrante__func__, TRACE_XML);
	if (est_premier(_terme_, uniteCourante)) {
		res = terme();
		exp = expArithBis(res);
		goto end;
	}
	
	erreur("");

end:	
	//affiche_balise_fermante(__func__, TRACE_XML);
	return exp;
}

n_exp * expArithBis(n_exp * herite)
{
	n_exp * exp = NULL;
	n_exp * herite_fils = NULL;
	n_exp * res = NULL;
	
	//affiche_balise_ouvrante__func__, TRACE_XML);
	if (uniteCourante == PLUS)  {
		afficher_sans_afficher();
		uniteCourante = yylex();
		res = terme();
		herite_fils = cree_n_exp_op(plus, herite, res);
		exp = expArithBis(herite_fils);
		goto end;
	}
	else if (uniteCourante == MOINS) {
		afficher_sans_afficher();
		uniteCourante = yylex();
		res = terme();
		herite_fils = cree_n_exp_op(moins, herite, res);
		exp = expArithBis(herite_fils);
		goto end;
	}
	else if (est_suivant(_expArithBis_, uniteCourante))
		exp = herite;
		goto end;
	
	erreur("");

end:
	//affiche_balise_fermante(__func__, TRACE_XML);
	return exp;
}

n_exp * terme(void)
{
	n_exp * exp = NULL;
	n_exp * res = NULL;
	//affiche_balise_ouvrante__func__, TRACE_XML);
	if (est_premier(_facteur_, uniteCourante)) {
		res = facteur();
		exp = termeBis(res);
		goto end;
	}
	erreur("");

end: 
	//affiche_balise_fermante(__func__, TRACE_XML);
	return exp;
}

n_exp * termeBis(n_exp * herite) {
	n_exp * exp = NULL;
	n_exp * res = NULL;
	n_exp * herite_fils = NULL;
	
	//affiche_balise_ouvrante__func__, TRACE_XML);
	if (uniteCourante == FOIS) {
		afficher_sans_afficher();
		uniteCourante = yylex();
		res = facteur();
		herite_fils = cree_n_exp_op(fois, herite, res);
		exp = termeBis(herite_fils);
		goto end;
	}
	else if (uniteCourante == DIVISE) {
		afficher_sans_afficher();
		uniteCourante = yylex();
		res = facteur();
		herite_fils = cree_n_exp_op(divise, herite, res);
		exp = termeBis(herite_fils);
		goto end;
	}
	else if (est_suivant(_termeBis_, uniteCourante))
		exp = herite;
		goto end;
	
	erreur("");
	
end: 
	//affiche_balise_fermante(__func__, TRACE_XML);
	return exp;
}

n_exp * facteur(void) {
	n_exp * exp;
	//affiche_balise_ouvrante__func__, TRACE_XML);
	if (uniteCourante == PARENTHESE_OUVRANTE) {
		afficher_sans_afficher();
		uniteCourante = yylex();
		exp = expression();
		if (uniteCourante == PARENTHESE_FERMANTE) {
			afficher_sans_afficher();
			uniteCourante = yylex();
			goto end;
		}
			
	}
	else if (uniteCourante == NOMBRE) {
		
		afficher_sans_afficher();
		char * val = duplique_chaine(valeur);
		int n = atoi(val);
		uniteCourante = yylex();
		exp = cree_n_exp_entier(n);
		goto end;
	}
	else if (est_premier(_appelFct_, uniteCourante)) {
		n_appel * appel = NULL;
		appel = appelFct();
		exp = cree_n_exp_appel(appel);
		goto end;
	}
	else if (est_premier(_var_, uniteCourante)) {
		n_var * vari = NULL;
		vari = var();
		exp = cree_n_exp_var(vari);
		goto end;
	}
	else if (uniteCourante == LIRE) {
		
		afficher_sans_afficher();
		uniteCourante = yylex();
		if (uniteCourante == PARENTHESE_OUVRANTE) {
			afficher_sans_afficher();
			uniteCourante = yylex();
			if (uniteCourante == PARENTHESE_FERMANTE)
				afficher_sans_afficher();
				uniteCourante = yylex();
				exp = cree_n_exp_lire();
				goto end;
		}
	}
	erreur("");

end:
	//affiche_balise_fermante(__func__, TRACE_XML);
	return exp;

}


n_var *  var(void)
{	n_var * var = NULL;
	n_exp * exp = NULL;
	//affiche_balise_ouvrante__func__, TRACE_XML);
	if (uniteCourante == ID_VAR) {
		
		afficher_sans_afficher();
		uniteCourante = yylex();
		char * nomVar = duplique_chaine(valeur);
		exp = optIndice();
		if (exp == NULL)
			var = cree_n_var_simple(nomVar);
		else 
			var = cree_n_var_indicee(nomVar, exp);
		goto end;
	}
	else
		erreur("");
end:
	//affiche_balise_fermante(__func__, TRACE_XML);
	return var;
	
}
n_exp * optIndice(void)
{ 
	n_exp * exp = NULL;
	//affiche_balise_ouvrante__func__, TRACE_XML);
	if (uniteCourante == CROCHET_OUVRANT) {
		afficher_sans_afficher();
		uniteCourante = yylex();
		exp = expression();
		if (uniteCourante == CROCHET_FERMANT) {
			afficher_sans_afficher();
			uniteCourante = yylex();
			goto end;
		}
	}
	else if (est_suivant(_optIndice_, uniteCourante))
		goto end;
	
	erreur("");
	
end:
	//affiche_balise_fermante(__func__, TRACE_XML);
	return exp;
}
n_appel * appelFct(void) 
{
	n_appel * appel = NULL;
	n_l_exp * args = NULL;
	char * nomFonc;
	//affiche_balise_ouvrante__func__, TRACE_XML); 
	if (uniteCourante == ID_FCT) {
		afficher_sans_afficher();
		nomFonc = duplique_chaine(valeur);
		uniteCourante = yylex();
		if (uniteCourante == PARENTHESE_OUVRANTE) {
			afficher_sans_afficher();
			uniteCourante = yylex();
			args = listeExpressions();
			if (uniteCourante == PARENTHESE_FERMANTE) {
				afficher_sans_afficher();
				uniteCourante = yylex();
				appel = cree_n_appel(nomFonc, args);
				goto end;
			}
				
			else 
				erreur("");
		}
		else
			erreur("");
	}
	else
		erreur("");

end: 
	//affiche_balise_fermante(__func__, TRACE_XML);
	return appel;
		
}
n_l_exp * listeExpressions(void) 
{
	n_l_exp * liste = NULL;
	n_exp * tete = NULL;
	n_l_exp * queue = NULL;
	//affiche_balise_ouvrante__func__, TRACE_XML);
	if (est_premier(_expression_, uniteCourante)){
		tete = expression();
		queue = listeExpressionsBis();
		liste = cree_n_l_exp(tete, queue);
		goto end;
	}
	else if (est_suivant(_listeExpressions_, uniteCourante)) 
		goto end;
	else 
		erreur("");
end:
	//affiche_balise_fermante(__func__, TRACE_XML);
	return liste;
}

n_l_exp * listeExpressionsBis(void) 
{
	n_l_exp * liste = NULL;
	n_exp * tete = NULL;
	n_l_exp * queue = NULL;
	//affiche_balise_ouvrante__func__, TRACE_XML);
	if (uniteCourante == VIRGULE) {
		afficher_sans_afficher();
		uniteCourante = yylex();
		tete = expression();
		queue = listeExpressionsBis();
		liste = cree_n_l_exp(tete, queue);
		goto end;
	}
	else if (est_suivant(_listeExpressionsBis_,uniteCourante)) {
		goto end;
	}
	else	
		erreur("");
end:
	//affiche_balise_fermante(__func__, TRACE_XML);	
	return liste;
}

	
int main(int argc, char **argv) {  
  n_prog * prog;
  yyin = fopen(argv[1], "r");
  if(yyin == NULL){
    fprintf(stderr, "impossible d'ouvrir le fichier %s\n", argv[1]);
    exit(1);
  }
  uniteCourante = yylex();
  prog = programme();
  if (uniteCourante != FIN) {
	erreur("");
  }
  /*printf("----------------------------------------------------------------------------------\n");
  printf("----------------------------------------------------------------------------------\n");
  printf("----------------------------------------------------------------------------------\n");*/
  construire_n_prog(prog);
  return 0;
}

