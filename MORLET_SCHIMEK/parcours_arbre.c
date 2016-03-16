#include <stdio.h>
#include "syntabs.h"
#include "util.h"
#include "dico.h"

#define AFFICHE_DICO 0

void parcourir_n_prog(n_prog *n);
void parcourir_l_instr(n_l_instr *n);
void parcourir_instr(n_instr *n);
void parcourir_instr_si(n_instr *n);
void parcourir_instr_tantque(n_instr *n);
void parcourir_instr_faire(n_instr *n);      /* MODIFIE POUR EVAL */
void parcourir_instr_pour(n_instr *n);       /* MODIFIE POUR EVAL */
void parcourir_instr_affect(n_instr *n);
void parcourir_instr_appel(n_instr *n);
void parcourir_instr_retour(n_instr *n);
void parcourir_instr_ecrire(n_instr *n);
int parcourir_l_exp(n_l_exp *n);
int parcourir_exp(n_exp *n);
int parcourir_varExp(n_exp *n);
int parcourir_opExp(n_exp *n);
int parcourir_intExp(n_exp *n);
int parcourir_lireExp(n_exp *n);
int parcourir_appelExp(n_exp *n);
int parcourir_l_dec(n_l_dec *n);
void parcourir_dec(n_dec *n);
void parcourir_foncDec(n_dec *n);
void parcourir_varDec(n_dec *n);
void parcourir_tabDec(n_dec *n);
int parcourir_var(n_var *n);
int parcourir_var_simple(n_var *n);
int parcourir_var_indicee(n_var *n);
int parcourir_appel(n_appel *n);


int adresseGlobalCourante = 0; // ajouté par moi ??
int adresseArgumentCourant = 0;
int adresseLocaleCourante = 0;
int contexte = C_VARIABLE_GLOBALE;

/*-------------------------------------------------------------------------*/
int allouer_reg() {
	static int n;
	return (n++ % 10);
}


void parcourir_n_prog(n_prog *n)
{
	printf(".data\n");
	parcourir_l_dec(n->variables);
	printf(".text\n");
	parcourir_l_dec(n->fonctions); 
	
	//il y a une fonction main()
	int i = rechercheExecutable("main");
	if (dico.tab[i].type != T_FONCTION || dico.tab[i].complement != 0)
		erreur("fonction main() n'est pas declarer");
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/

void parcourir_l_instr(n_l_instr *n)
{
  char *fct = "l_instr";
  if(n){
  //parcourir_balise_ouvrante(fct, trace_abs);
  parcourir_instr(n->tete);
  parcourir_l_instr(n->queue);
  //parcourir_balise_fermante(fct, trace_abs);
  }
}

/*-------------------------------------------------------------------------*/

void parcourir_instr(n_instr *n)
{
  if(n){
    if(n->type == blocInst) parcourir_l_instr(n->u.liste);
    else if(n->type == affecteInst) parcourir_instr_affect(n);
    else if(n->type == siInst) parcourir_instr_si(n);
    else if(n->type == tantqueInst) parcourir_instr_tantque(n);
    else if(n->type == faireInst) parcourir_instr_faire(n);
    else if(n->type == pourInst) parcourir_instr_pour(n);    
    else if(n->type == appelInst) parcourir_instr_appel(n);
    else if(n->type == retourInst) parcourir_instr_retour(n);
    else if(n->type == ecrireInst) parcourir_instr_ecrire(n);
  }
}

/*-------------------------------------------------------------------------*/

void parcourir_instr_si(n_instr *n)
{  
  char *fct = "instr_si";
  //parcourir_balise_ouvrante(fct, trace_abs);

  parcourir_exp(n->u.si_.test);
  parcourir_instr(n->u.si_.alors);
  if(n->u.si_.sinon){
    parcourir_instr(n->u.si_.sinon);
  }
  //parcourir_balise_fermante(fct, trace_abs);
}

/*-------------------------------------------------------------------------*/

void parcourir_instr_tantque(n_instr *n)
{
  char *fct = "instr_tantque";
  //parcourir_balise_ouvrante(fct, trace_abs);

  parcourir_exp(n->u.tantque_.test);
  parcourir_instr(n->u.tantque_.faire);
  //parcourir_balise_fermante(fct, trace_abs);
}

/*-------------------------------------------------------------------------*/

void parcourir_instr_faire(n_instr *n)          /* MODIFIE POUR EVAL */
{                                             /* MODIFIE POUR EVAL */
  char *fct = "instr_faire";                  /* MODIFIE POUR EVAL */
  //parcourir_balise_ouvrante(fct, trace_abs);    /* MODIFIE POUR EVAL */
  parcourir_instr(n->u.faire_.faire);           /* MODIFIE POUR EVAL */
  parcourir_exp(n->u.faire_.test);              /* MODIFIE POUR EVAL */
  //parcourir_balise_fermante(fct, trace_abs);    /* MODIFIE POUR EVAL */
}                                             /* MODIFIE POUR EVAL */

/*-------------------------------------------------------------------------*/

void parcourir_instr_pour(n_instr *n)                /* MODIFIE POUR EVAL */
{                                                  /* MODIFIE POUR EVAL */
  char *fct = "instr_pour";                        /* MODIFIE POUR EVAL */
  //parcourir_balise_ouvrante(fct, trace_abs);         /* MODIFIE POUR EVAL */
  parcourir_instr(n->u.pour_.init);                  /* MODIFIE POUR EVAL */
  parcourir_exp(n->u.pour_.test);                    /* MODIFIE POUR EVAL */
  parcourir_instr(n->u.pour_.faire);                 /* MODIFIE POUR EVAL */
  parcourir_instr(n->u.pour_.incr);                  /* MODIFIE POUR EVAL */
  //parcourir_balise_fermante(fct, trace_abs);         /* MODIFIE POUR EVAL */
}                                                  /* MODIFIE POUR EVAL */

/*-------------------------------------------------------------------------*/

void parcourir_instr_affect(n_instr *n)
{
	// validité de la variable est déjà verifiée
	int reg1 = parcourir_var(n->u.affecte_.var);
	int reg2 = parcourir_exp(n->u.affecte_.exp);
	
	//affiche_instr_affect(n);
	int i = rechercheExecutable(n->u.affecte_.var->nom);
	if (n->u.affecte_.var->type == simple) {
		// variable simple
		int adresse = dico.tab[i].adresse;
		int reg3 = allouer_reg();
		printf("la $t%d, %s\n",reg3,dico.tab[i].identif);
		printf("sw $t%d ($t%d)\n", reg2, reg3);
	}
	else {
		// tableau
		int adresse = dico.tab[i].adresse;
		int reg3 = allouer_reg();  
		printf("li $t%d, 4\n", reg3);  // $t3 == 4
		printf("mult $t%d, $t%d\n", reg2, reg3);
		printf("mflo %t%d\n",reg2);
		printf("addi $t%d, $t%d, %d\n", reg2, reg2, adresse);
		printf("sw ($t%d) %d\n", reg1, adresse);
	}
  
}

/*-------------------------------------------------------------------------*/

void parcourir_instr_appel(n_instr *n)
{
  char *fct = "instr_appel";
  //parcourir_balise_ouvrante(fct, trace_abs);


  parcourir_appel(n->u.appel);
  //parcourir_balise_fermante(fct, trace_abs);
}
/*-------------------------------------------------------------------------*/

int parcourir_appel(n_appel *n)
{
	int nbArg = 0;
	int i = -1;
	nbArg = parcourir_l_exp(n->args);
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
	
	// pour l'instant
	return -1;
}

/*-------------------------------------------------------------------------*/

void parcourir_instr_retour(n_instr *n)
{
  char *fct = "instr_retour";
  //parcourir_balise_ouvrante(fct, trace_abs);
  parcourir_exp(n->u.retour_.expression);
  //parcourir_balise_fermante(fct, trace_abs);

}

/*-------------------------------------------------------------------------*/

void parcourir_instr_ecrire(n_instr *n)
{
  char *fct = "instr_ecrire";
  int reg = parcourir_exp(n->u.ecrire_.expression);
  printf("li $v0, 1\t\t\n");
  printf("move $a0, $t%d\n", reg);
  printf("syscall\t\t#imprimer registre $t%d\n", reg);
}

/*-------------------------------------------------------------------------*/

int parcourir_l_exp(n_l_exp *n)
{
	int nombre = 1;
	//char *fct = "l_exp";
	//parcourir_balise_ouvrante(fct, trace_abs);
	if (n == NULL)
		return 0;
	if(n){
		parcourir_exp(n->tete);
		nombre += parcourir_l_exp(n->queue);
	}
	//parcourir_balise_fermante(fct, trace_abs);
	return nombre;
}

/*-------------------------------------------------------------------------*/

int parcourir_exp(n_exp *n)
{
	int numero = -1;
	if(n->type == varExp) 
		numero = parcourir_varExp(n);
	else if(n->type == opExp) 
		numero = parcourir_opExp(n);
	else if(n->type == intExp) 
		numero = parcourir_intExp(n);
	else if(n->type == appelExp) 
		numero =parcourir_appelExp(n);
	else if(n->type == lireExp) 
		numero = parcourir_lireExp(n);

	return numero;
}

/*-------------------------------------------------------------------------*/

int parcourir_varExp(n_exp *n)
{
  char *fct = "varExp";
  //parcourir_balise_ouvrante(fct, trace_abs);
  return parcourir_var(n->u.var);
  //parcourir_balise_fermante(fct, trace_abs);
}

/*-------------------------------------------------------------------------*/
int parcourir_opExp(n_exp *n)
{
	int numero1, numero2;
	if( n->u.opExp_.op1 != NULL ) {
		numero1 = parcourir_exp(n->u.opExp_.op1);
	}
	if( n->u.opExp_.op2 != NULL ) {
		numero2 = parcourir_exp(n->u.opExp_.op2);
	}
	//parcourir_balise_ouvrante(fct, trace_abs);
	if(n->u.opExp_.op == plus) {
		printf("add $t%d, $t%d, $t%d\n",numero1, numero1, numero2);
		return numero1;
	}
	else if(n->u.opExp_.op == moins) {
		printf("sub $t%d, $t%d, $t%d\n",numero1, numero1, numero2);
		return numero1;
	}
	else if(n->u.opExp_.op == fois) {
		printf("mult $t%d, $t%d\n",numero1, numero2);
		printf("mflo $t%d\n",numero1);
		return numero1;
	}
	else if(n->u.opExp_.op == divise){
		printf("divide $t%d, $t%d\n",numero1, numero2);
		printf("mflo $t%d\n",numero1);
		return numero1;
	}
	/***********************************************
	 *  VRAI == 1 ????
	 *  FAUX == 0 ????
	 *************************************************/
	else if(n->u.opExp_.op == egal){ 
		return -1;
	}
	else if(n->u.opExp_.op == diff) {
		return -1;
	}
	else if(n->u.opExp_.op == inf) {
		printf("slt $t%d, $t%d\n",numero1, numero1, numero2);
		return numero1;
	}
	else if(n->u.opExp_.op == infeg) {
		return -1;
	}
	else if(n->u.opExp_.op == ou) {
		printf("or $t%d, $t%d\n",numero1, numero1, numero2);
		return numero1;
	}
	else if(n->u.opExp_.op == et) {
		printf("and $t%d, $t%d\n",numero1, numero1, numero2);
		return numero1;
	}
	else if(n->u.opExp_.op == non) {
		printf("nor $t%d, $t%d\n",numero1, numero1, numero1);
		return numero1;
	}  

//parcourir_balise_fermante(fct, trace_abs);
}

/*-------------------------------------------------------------------------*/

int parcourir_intExp(n_exp *n)
{
  char texte[ 50 ]; // Max. 50 chiffres
  int reg = allouer_reg();
  sprintf(texte, "%d", n->u.entier);
  printf("li $t%d, %s\n", reg, texte);
  return reg;
}

/*-------------------------------------------------------------------------*/
int parcourir_lireExp(n_exp *n)
{
  
  int reg = allouer_reg();
  printf("li $v0, 5\n");
  printf("syscall\t\t# lit valeur depuis utilisateur\n");
  printf("move $t%d, $v0\t# recopier valeur lu dans registre \n", reg);
  return reg;

}

/*-------------------------------------------------------------------------*/

int parcourir_appelExp(n_exp *n)
{
  char *fct = "appelExp";
  //parcourir_balise_ouvrante(fct, trace_abs);
  return parcourir_appel(n->u.appel);
  //parcourir_balise_fermante(fct, trace_abs);
}

/*-------------------------------------------------------------------------*/

int parcourir_l_dec(n_l_dec *n)
{
  if (n == NULL)
	  return 0;
  
  char *fct = "l_dec";
  int nombre = 1;
  if( n ){
    //parcourir_balise_ouvrante(fct, trace_abs);
    parcourir_dec(n->tete);
    nombre += parcourir_l_dec(n->queue);
    //parcourir_balise_fermante(fct, trace_abs);
  }
  return nombre;
}

/*-------------------------------------------------------------------------*/

void parcourir_dec(n_dec *n)
{

  if(n){
    if(n->type == foncDec) {
      parcourir_foncDec(n);
    }
    else if(n->type == varDec) {
      parcourir_varDec(n);
	 
    }
    else if(n->type == tabDec) { 
      parcourir_tabDec(n);
    }
  }
}

/*-------------------------------------------------------------------------*/

void parcourir_foncDec(n_dec *n)
{
	int i = rechercheExecutable(n->nom);
	if (i == -1) {
		i = ajouteIdentificateur(n->nom, C_VARIABLE_GLOBALE, T_FONCTION, 0, 0);
		entreeFonction();
		contexte = C_ARGUMENT;
		int nombre = parcourir_l_dec(n->u.foncDec_.param);
		contexte = C_VARIABLE_LOCALE;
		parcourir_l_dec(n->u.foncDec_.variables);
		parcourir_instr(n->u.foncDec_.corps);
		dico.tab[i].complement = nombre;
#if AFFICHE_DICO
		affiche_dico();
#endif 
		sortieFonction();
	}
	else {
		erreur("fonction déjà déclarée");
	}
}
/*-------------------------------------------------------------------------*/

void parcourir_varDec(n_dec *n)
{
//parcourir_element("varDec", n->nom, trace_abs);
	if (contexte == C_VARIABLE_GLOBALE) {
		int i = rechercheDeclarative(n->nom);
		if (i == -1) {
			ajouteIdentificateur(n->nom, C_VARIABLE_GLOBALE, T_ENTIER, adresseGlobalCourante, -1);
			adresseGlobalCourante += 4;
			printf("%s: .space 4\n", n->nom);
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

void parcourir_tabDec(n_dec *n)
{
	//char texte[100]; // Max. 100 chars nom tab + taille
	//sprintf(texte, "%s[%d]", n->nom, n->u.tabDec_.taille);
	//parcourir_element( "tabDec", texte, trace_abs );
	if (contexte == C_VARIABLE_GLOBALE) {
		int i = rechercheDeclarative(n->nom);
		if (i == -1) {
			ajouteIdentificateur(n->nom, C_VARIABLE_GLOBALE, T_TABLEAU_ENTIER, adresseGlobalCourante, n->u.tabDec_.taille);
			adresseGlobalCourante +=4;
			printf("%s: .space %d\n",n->nom, n->u.tabDec_.taille * 4);
		}
		else {
			erreur("Variable déjà declarée");
		}
	}
}

/*-------------------------------------------------------------------------*/

int parcourir_var(n_var *n)
{
	
	if(n->type == simple) {
		return parcourir_var_simple(n);
	}
	else if(n->type == indicee) {
		return parcourir_var_indicee(n);
	}
	
}

/*-------------------------------------------------------------------------*/
int parcourir_var_simple(n_var *n)
{
	// on ne cherche pas encore en l'ordre
	//parcourir_element("var_simple", n->nom, trace_abs);
	int i = rechercheDeclarative(n->nom);
	if (i == -1) {
		i = rechercheExecutable(n->nom);
		if (i == -1)
			erreur("variable n'est pas déclarée");
		else if (dico.tab[i].type != T_ENTIER) {
			erreur("nom utilisée n'est pas une variable de type entier");
		}
		int reg = allouer_reg();
		printf("la $t%d, %s\t#charge adresse de label\n",reg, n->nom);
		printf("lw $t%d, ($t%d)\t# lit variable dans registre\n",reg,reg);
		//printf("lw $t%d, %d\t# lit variable dans registre\n", reg, dico.tab[i].adresse);
		return reg;
	}
	else {
		return -1;
	}
}

/*-------------------------------------------------------------------------*/
int parcourir_var_indicee(n_var *n)
{
	/*char *fct = "var_indicee";
	parcourir_balise_ouvrante(fct, trace_abs);
	parcourir_element("var_base_tableau", n->nom, trace_abs);
	parcourir_exp( n->u.indicee_.indice );
	parcourir_balise_fermante(fct, trace_abs); */
	int i = rechercheExecutable(n->nom);
	if (i >= 0) {
		if (dico.tab[i].type != T_TABLEAU_ENTIER)
			erreur("nom utilisée n'est pas une variable de type tableau entier");
	}
	else 
		erreur("nom utilisée n'est pas declarée");
	int indice = parcourir_exp( n->u.indicee_.indice );
	int reg1 = allouer_reg();    // $t1
	int reg2 = allouer_reg();   // $t2

	printf("lw $t%d, %d\n", reg1, dico.tab[i].adresse);
	printf("li $t%d, 4\n",reg2);  // $t2 = 4;
	printf("mult $t%d, $t%d\n", indice, reg2);
	printf("mflo $t%d\n",reg2);
	printf("add $t%d, $t%d, $t%d\n",reg2, reg2, reg1);
	printf("lw $t%d, ($t%d)\n",reg1, reg2);
	return reg1;
		
}
/*-------------------------------------------------------------------------*/
