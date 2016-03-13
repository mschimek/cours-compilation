#include <stdio.h>
#include "syntabs.h"
#include "util.h"
#include "dico.h"

void construire_n_prog(n_prog *n);
void construire_l_instr(n_l_instr *n);
void construire_instr(n_instr *n);
void construire_instr_si(n_instr *n);
void construire_instr_tantque(n_instr *n);
void construire_instr_faire(n_instr *n);      /* MODIFIE POUR EVAL */
void construire_instr_pour(n_instr *n);       /* MODIFIE POUR EVAL */
void construire_instr_affect(n_instr *n);
void construire_instr_appel(n_instr *n);
void construire_instr_retour(n_instr *n);
void construire_instr_ecrire(n_instr *n);
int construire_l_exp(n_l_exp *n);
void construire_exp(n_exp *n);
void construire_varExp(n_exp *n);
void construire_opExp(n_exp *n);
void construire_intExp(n_exp *n);
void construire_lireExp(n_exp *n);
void construire_appelExp(n_exp *n);
int construire_l_dec(n_l_dec *n);
void construire_dec(n_dec *n);
void construire_foncDec(n_dec *n);
void construire_varDec(n_dec *n);
void construire_tabDec(n_dec *n);
int construire_var(n_var *n);
void construire_var_simple(n_var *n);
void construire_var_indicee(n_var *n);
void construire_appel(n_appel *n);


int adresseGlobalCourante = 0; // ajouté par moi ??
int adresseArgumentCourant = 0;
int adresseLocaleCourante = 0;
int contexte = C_VARIABLE_GLOBALE;

/*-------------------------------------------------------------------------*/

void construire_n_prog(n_prog *n)
{
	char *fct = "prog";
	//construire_balise_ouvrante(fct, trace_abs);
	
	construire_l_dec(n->variables);
	construire_l_dec(n->fonctions); 
	
	//il y a une fonction main()
	int i = rechercheExecutable("main");
	if (dico.tab[i].type != T_FONCTION || dico.tab[i].complement != 0)
		erreur("fonction main() n'est pas declarer");
  //construire_balise_fermante(fct, trace_abs);
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/

void construire_l_instr(n_l_instr *n)
{
  char *fct = "l_instr";
  if(n){
  //construire_balise_ouvrante(fct, trace_abs);
  construire_instr(n->tete);
  construire_l_instr(n->queue);
  //construire_balise_fermante(fct, trace_abs);
  }
}

/*-------------------------------------------------------------------------*/

void construire_instr(n_instr *n)
{
  if(n){
    if(n->type == blocInst) construire_l_instr(n->u.liste);
    else if(n->type == affecteInst) construire_instr_affect(n);
    else if(n->type == siInst) construire_instr_si(n);
    else if(n->type == tantqueInst) construire_instr_tantque(n);
    else if(n->type == faireInst) construire_instr_faire(n);
    else if(n->type == pourInst) construire_instr_pour(n);    
    else if(n->type == appelInst) construire_instr_appel(n);
    else if(n->type == retourInst) construire_instr_retour(n);
    else if(n->type == ecrireInst) construire_instr_ecrire(n);
  }
}

/*-------------------------------------------------------------------------*/

void construire_instr_si(n_instr *n)
{  
  char *fct = "instr_si";
  //construire_balise_ouvrante(fct, trace_abs);

  construire_exp(n->u.si_.test);
  construire_instr(n->u.si_.alors);
  if(n->u.si_.sinon){
    construire_instr(n->u.si_.sinon);
  }
  //construire_balise_fermante(fct, trace_abs);
}

/*-------------------------------------------------------------------------*/

void construire_instr_tantque(n_instr *n)
{
  char *fct = "instr_tantque";
  //construire_balise_ouvrante(fct, trace_abs);

  construire_exp(n->u.tantque_.test);
  construire_instr(n->u.tantque_.faire);
  //construire_balise_fermante(fct, trace_abs);
}

/*-------------------------------------------------------------------------*/

void construire_instr_faire(n_instr *n)          /* MODIFIE POUR EVAL */
{                                             /* MODIFIE POUR EVAL */
  char *fct = "instr_faire";                  /* MODIFIE POUR EVAL */
  //construire_balise_ouvrante(fct, trace_abs);    /* MODIFIE POUR EVAL */
  construire_instr(n->u.faire_.faire);           /* MODIFIE POUR EVAL */
  construire_exp(n->u.faire_.test);              /* MODIFIE POUR EVAL */
  //construire_balise_fermante(fct, trace_abs);    /* MODIFIE POUR EVAL */
}                                             /* MODIFIE POUR EVAL */

/*-------------------------------------------------------------------------*/

void construire_instr_pour(n_instr *n)                /* MODIFIE POUR EVAL */
{                                                  /* MODIFIE POUR EVAL */
  char *fct = "instr_pour";                        /* MODIFIE POUR EVAL */
  //construire_balise_ouvrante(fct, trace_abs);         /* MODIFIE POUR EVAL */
  construire_instr(n->u.pour_.init);                  /* MODIFIE POUR EVAL */
  construire_exp(n->u.pour_.test);                    /* MODIFIE POUR EVAL */
  construire_instr(n->u.pour_.faire);                 /* MODIFIE POUR EVAL */
  construire_instr(n->u.pour_.incr);                  /* MODIFIE POUR EVAL */
  //construire_balise_fermante(fct, trace_abs);         /* MODIFIE POUR EVAL */
}                                                  /* MODIFIE POUR EVAL */

/*-------------------------------------------------------------------------*/

void construire_instr_affect(n_instr *n)
{

  char *fct = "instr_affect";
  //construire_balise_ouvrante(fct, trace_abs);


  construire_var(n->u.affecte_.var);
  construire_exp(n->u.affecte_.exp);
  //construire_balise_fermante(fct, trace_abs);
}

/*-------------------------------------------------------------------------*/

void construire_instr_appel(n_instr *n)
{
  char *fct = "instr_appel";
  //construire_balise_ouvrante(fct, trace_abs);


  construire_appel(n->u.appel);
  //construire_balise_fermante(fct, trace_abs);
}
/*-------------------------------------------------------------------------*/

void construire_appel(n_appel *n)
{
	int nbArg = 0;
	int i = -1;
	//char *fct = "appel";
	//construire_balise_ouvrante(fct, trace_abs);
	//construire_texte( n->fonction, trace_abs);
	nbArg = construire_l_exp(n->args);
	i = rechercheExecutable(n->fonction);
	if (i == -1) {
		erreur("fonction n'est pas declarée");
	}
	else if (dico.tab[i].type != T_FONCTION) {
		erreur("identificateur n'est pas une fonction");
	}
	else if(dico.tab[i].complement != nbArg) {
		printf("n: fonction %s\n", n->fonction);
		printf("n: arg %d\n", nbArg);
		erreur("fonction %s ne satisfait pas le type necessaire");
	}
  //construire_balise_fermante(fct, trace_abs);
}

/*-------------------------------------------------------------------------*/

void construire_instr_retour(n_instr *n)
{
  char *fct = "instr_retour";
  //construire_balise_ouvrante(fct, trace_abs);
  construire_exp(n->u.retour_.expression);
  //construire_balise_fermante(fct, trace_abs);

}

/*-------------------------------------------------------------------------*/

void construire_instr_ecrire(n_instr *n)
{
  char *fct = "instr_ecrire";
  //construire_balise_ouvrante(fct, trace_abs);
  construire_exp(n->u.ecrire_.expression);
  //construire_balise_fermante(fct, trace_abs);
}

/*-------------------------------------------------------------------------*/

int construire_l_exp(n_l_exp *n)
{
	int nombre = 1;
	//char *fct = "l_exp";
	//construire_balise_ouvrante(fct, trace_abs);
	if (n == NULL)
		return 0;
	if(n){
		construire_exp(n->tete);
		nombre += construire_l_exp(n->queue);
	}
	//construire_balise_fermante(fct, trace_abs);
	return nombre;
}

/*-------------------------------------------------------------------------*/

void construire_exp(n_exp *n)
{

  if(n->type == varExp) construire_varExp(n);
  else if(n->type == opExp) construire_opExp(n);
  else if(n->type == intExp) construire_intExp(n);
  else if(n->type == appelExp) construire_appelExp(n);
  else if(n->type == lireExp) construire_lireExp(n);
}

/*-------------------------------------------------------------------------*/

void construire_varExp(n_exp *n)
{
  char *fct = "varExp";
  //construire_balise_ouvrante(fct, trace_abs);
  construire_var(n->u.var);
  //construire_balise_fermante(fct, trace_abs);
}

/*-------------------------------------------------------------------------*/
void construire_opExp(n_exp *n)
{
  char *fct = "opExp";
  //construire_balise_ouvrante(fct, trace_abs);
  /*if(n->u.opExp_.op == plus) construire_texte("plus", trace_abs);
  else if(n->u.opExp_.op == moins) construire_texte("moins", trace_abs);
  else if(n->u.opExp_.op == fois) construire_texte("fois", trace_abs);
  else if(n->u.opExp_.op == divise) construire_texte("divise", trace_abs);
  else if(n->u.opExp_.op == egal) construire_texte("egal", trace_abs);
  else if(n->u.opExp_.op == diff) construire_texte("diff", trace_abs);
  else if(n->u.opExp_.op == inf) construire_texte("inf", trace_abs);
  else if(n->u.opExp_.op == infeg) construire_texte("infeg", trace_abs);
  else if(n->u.opExp_.op == ou) construire_texte("ou", trace_abs);
  else if(n->u.opExp_.op == et) construire_texte("et", trace_abs);
  else if(n->u.opExp_.op == non) construire_texte("non", trace_abs);   */
  if( n->u.opExp_.op1 != NULL ) {
    construire_exp(n->u.opExp_.op1);
  }
  if( n->u.opExp_.op2 != NULL ) {
    construire_exp(n->u.opExp_.op2);
  }
  //construire_balise_fermante(fct, trace_abs);
}

/*-------------------------------------------------------------------------*/

void construire_intExp(n_exp *n)
{
  char texte[ 50 ]; // Max. 50 chiffres
  sprintf(texte, "%d", n->u.entier);
  //construire_element( "intExp", texte, trace_abs );
}

/*-------------------------------------------------------------------------*/
void construire_lireExp(n_exp *n)
{
  char *fct = "lireExp";
  //construire_balise_ouvrante(fct, trace_abs);
  //construire_balise_fermante(fct, trace_abs);

}

/*-------------------------------------------------------------------------*/

void construire_appelExp(n_exp *n)
{
  char *fct = "appelExp";
  //construire_balise_ouvrante(fct, trace_abs);
  construire_appel(n->u.appel);
  //construire_balise_fermante(fct, trace_abs);
}

/*-------------------------------------------------------------------------*/

int construire_l_dec(n_l_dec *n)
{
  if (n == NULL)
	  return 0;
  
  char *fct = "l_dec";
  int nombre = 1;
  if( n ){
    //construire_balise_ouvrante(fct, trace_abs);
    construire_dec(n->tete);
    nombre += construire_l_dec(n->queue);
    //construire_balise_fermante(fct, trace_abs);
  }
  return nombre;
}

/*-------------------------------------------------------------------------*/

void construire_dec(n_dec *n)
{

  if(n){
    if(n->type == foncDec) {
      construire_foncDec(n);
    }
    else if(n->type == varDec) {
      construire_varDec(n);
    }
    else if(n->type == tabDec) { 
      construire_tabDec(n);
    }
  }
}

/*-------------------------------------------------------------------------*/

void construire_foncDec(n_dec *n)
{
	int i = rechercheExecutable(n->nom);
	if (i == -1) {
		i = ajouteIdentificateur(n->nom, C_VARIABLE_GLOBALE, T_FONCTION, 0, 0);
		entreeFonction();
		contexte = C_ARGUMENT;
		int nombre = construire_l_dec(n->u.foncDec_.param);
		contexte = C_VARIABLE_LOCALE;
		construire_l_dec(n->u.foncDec_.variables);
		construire_instr(n->u.foncDec_.corps);
		dico.tab[i].complement = nombre;
		affiche_dico();
		sortieFonction();
	}
	else {
		erreur("fonction déjà déclarée");
	}
}
/*-------------------------------------------------------------------------*/

void construire_varDec(n_dec *n)
{
//construire_element("varDec", n->nom, trace_abs);
	if (contexte == C_VARIABLE_GLOBALE) {
		int i = rechercheDeclarative(n->nom);
		if (i == -1) {
		ajouteIdentificateur(n->nom, C_VARIABLE_GLOBALE, T_ENTIER, adresseGlobalCourante, -1);
		adresseGlobalCourante += 4;
		}
		else {
			erreur("Variable déjà declarée");
		}
	}
	else if (contexte == C_VARIABLE_LOCALE) {
		int i = rechercheDeclarative(n->nom);
		if (i == -1) {
			ajouteIdentificateur(n->nom, C_VARIABLE_LOCALE, T_ENTIER, adresseLocaleCourante, -1);
			adresseLocaleCourante += 4;
		}
		else {
		erreur("Variable déjà declarée");
		}	
	}
	else {
		int i = rechercheDeclarative(n->nom);
		if (i == -1) {
			ajouteIdentificateur(n->nom, C_ARGUMENT, T_ENTIER, adresseArgumentCourant, -1);
			adresseArgumentCourant += 4;
		}
		else {
			erreur("Nom d'argument utilisée");
		}
	}
}

/*-------------------------------------------------------------------------*/

void construire_tabDec(n_dec *n)
{
	//char texte[100]; // Max. 100 chars nom tab + taille
	//sprintf(texte, "%s[%d]", n->nom, n->u.tabDec_.taille);
	//construire_element( "tabDec", texte, trace_abs );
	if (contexte == C_VARIABLE_GLOBALE) {
		int i = rechercheDeclarative(n->nom);
		if (i == -1) {
			ajouteIdentificateur(n->nom, C_VARIABLE_GLOBALE, T_TABLEAU_ENTIER, adresseGlobalCourante, n->u.tabDec_.taille);
			adresseGlobalCourante +=4;
		}
		else {
			erreur("Variable déjà declarée");
		}
	}
}

/*-------------------------------------------------------------------------*/

int construire_var(n_var *n)
{
	
	if(n->type == simple) {
		construire_var_simple(n);
	}
	else if(n->type == indicee) {
		construire_var_indicee(n);
	}
	return T_ENTIER;
}

/*-------------------------------------------------------------------------*/
void construire_var_simple(n_var *n)
{
	// on ne cherche pas encore en l'ordre
	//construire_element("var_simple", n->nom, trace_abs);
	int i = rechercheDeclarative(n->nom);
	if (i == -1) {
		i = rechercheExecutable(n->nom);
		if (i == -1)
			erreur("variable n'est pas déclarée");
		else if (dico.tab[i].type != T_ENTIER) {
			erreur("nom utilisée n'est pas une variable de type entier");
		}
	}
}

/*-------------------------------------------------------------------------*/
void construire_var_indicee(n_var *n)
{
	/*char *fct = "var_indicee";
	construire_balise_ouvrante(fct, trace_abs);
	construire_element("var_base_tableau", n->nom, trace_abs);
	construire_exp( n->u.indicee_.indice );
	construire_balise_fermante(fct, trace_abs); */
	int i = rechercheExecutable(n->nom);
	if (i >= 0) {
		if (dico.tab[i].type != T_TABLEAU_ENTIER)
			erreur("nom utilisée n'est pas une variable de type tableau entier");
	}
	else 
		erreur("nom utilisée n'est pas declarée");
		
}
/*-------------------------------------------------------------------------*/
