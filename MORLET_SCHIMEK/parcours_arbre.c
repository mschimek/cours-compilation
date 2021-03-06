#include <stdio.h>
#include "syntabs.h"
#include "util.h"
#include "dico.h"

#define AFFICHE_DICO 0

void parcourir_n_prog(n_prog * n);
void parcourir_l_instr(n_l_instr * n);
void parcourir_instr(n_instr * n);
void parcourir_instr_si(n_instr * n);
void parcourir_instr_tantque(n_instr * n);
void parcourir_instr_faire(n_instr * n);	/* MODIFIE POUR EVAL */
void parcourir_instr_pour(n_instr * n);	/* MODIFIE POUR EVAL */
void parcourir_instr_affect(n_instr * n);
void parcourir_instr_appel(n_instr * n);
void parcourir_instr_retour(n_instr * n);
void parcourir_instr_ecrire(n_instr * n);
int parcourir_l_exp(n_l_exp * n);
int parcourir_exp(n_exp * n);
int parcourir_varExp(n_exp * n);
int parcourir_opExp(n_exp * n);
int parcourir_intExp(n_exp * n);
int parcourir_lireExp(n_exp * n);
int parcourir_appelExp(n_exp * n);
int parcourir_l_dec(n_l_dec * n);
void parcourir_dec(n_dec * n);
void parcourir_foncDec(n_dec * n);
void parcourir_varDec(n_dec * n);
void parcourir_tabDec(n_dec * n);
int parcourir_var(n_var * n);
int parcourir_var_simple(n_var * n, int empiler_Valeur);
int parcourir_var_indicee(n_var * n, int empiler_Valeur);
int parcourir_appel(n_appel * n);


int adresseGlobalCourante = 0;	// ajouté par moi ??
int adresseArgumentCourant = 0;
int adresseLocaleCourante = 0;
int contexte = C_VARIABLE_GLOBALE;

void empiler_registre(char reg, char num)
{
    printf("\tsubi $sp, $sp, 4\t# empile registre\n");
    printf("\tsw $%c%c, 0($sp)\n", reg, num);

}

void depiler_registre(char reg, char num)
{


    printf("\tlw $%c%c, 0($sp)\t\t# depile vers registre\n", reg, num);

    printf("\taddi $sp, $sp, 4\n");
}

int nb_variable_locale()
{
    int counter = 0;
    int i = dico.sommet - 1;
    while (dico.tab[i].classe == C_VARIABLE_LOCALE) {
	i--;
	counter++;
    }
    return counter;
}

int nb_argument()
{
    int nb_local = nb_variable_locale();
    int counter = 0;
    int i = dico.sommet - 1 - nb_local;
    while (dico.tab[i].classe == C_ARGUMENT) {
	i--;
	counter++;
    }
    return counter;
}

/*-------------------------------------------------------------------------*/
int allouer_reg()
{
    static int n;
    return (n++ % 10);
}

void get_label(char *id, char *str)
{
    static int n;
    sprintf(str, "%s%d", id, n++);
}


void parcourir_n_prog(n_prog * n)
{
    printf(".data\n");
    parcourir_l_dec(n->variables);
    printf(".text\n");
    printf("__start:\n");
    printf("\tjal main\n");
    printf("\tli $v0, 10\n");
    printf("\tsyscall\t\t # stoppe l'exectuion du processus\n");
    parcourir_l_dec(n->fonctions);



    //il y a une fonction main()
    int i = rechercheExecutable("main");
    if (dico.tab[i].type != T_FONCTION || dico.tab[i].complement != 0)
	erreur("fonction main() n'est pas declarer");
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/

void parcourir_l_instr(n_l_instr * n)
{
    char *fct = "l_instr";
    if (n) {
	parcourir_instr(n->tete);
	parcourir_l_instr(n->queue);
    }
}

/*-------------------------------------------------------------------------*/

void parcourir_instr(n_instr * n)
{
    if (n) {
	if (n->type == blocInst)
	    parcourir_l_instr(n->u.liste);
	else if (n->type == affecteInst)
	    parcourir_instr_affect(n);
	else if (n->type == siInst)
	    parcourir_instr_si(n);
	else if (n->type == tantqueInst)
	    parcourir_instr_tantque(n);
	else if (n->type == faireInst)
	    parcourir_instr_faire(n);
	else if (n->type == pourInst)
	    parcourir_instr_pour(n);
	else if (n->type == appelInst)
	    parcourir_instr_appel(n);
	else if (n->type == retourInst)
	    parcourir_instr_retour(n);
	else if (n->type == ecrireInst)
	    parcourir_instr_ecrire(n);
    }
}

/*-------------------------------------------------------------------------*/

void parcourir_instr_si(n_instr * n)
{
    char *fct = "instr_si";

    parcourir_exp(n->u.si_.test);
    char reg = 't';
    char num0 = '0';
    char num1 = '1';
    char suite[20];
    char sinon[20];
    get_label("suite", suite);
    depiler_registre(reg, num1);
    if (n->u.si_.sinon) {
	get_label("sinon", sinon);
	printf("\tli $%c%c, -1\t\t# si - instruction\n", reg, num0);
	printf("\tbne $%c%c, $%c%c, %s\n", reg, num1, reg, num0, sinon);
	parcourir_instr(n->u.si_.alors);
	printf("\tj %s\n", suite);
    } else {
	printf("\tli $%c%c, -1\t\t# si - instruction\n", reg, num0);
	printf("\tbne $%c%c, $%c%c, %s\n", reg, num1, reg, num0, suite);
	parcourir_instr(n->u.si_.alors);
	printf("\tj %s\n", suite);
    }
    if (n->u.si_.sinon) {
	printf("%s:\n", sinon);
	parcourir_instr(n->u.si_.sinon);
    }
    printf("%s:\n", suite);
}

/*-------------------------------------------------------------------------*/

void parcourir_instr_tantque(n_instr * n)
{
    char *fct = "instr_tantque";
    char test[20];
    char suite[20];
    get_label("test", test);
    get_label("suite", suite);
    char reg = 't';
    char num0 = '0';
    char num1 = '1';
    printf("%s:\n", test);
    parcourir_exp(n->u.tantque_.test);
    depiler_registre(reg, num0);
    printf("\tli $%c%c, -1\t\t# tant que\n", reg, num1);
    printf("\tbne $%c%c, $%c%c, %s\n", reg, num0, reg, num1, suite);
    parcourir_instr(n->u.tantque_.faire);
    printf("\tj %s\n", test);
    printf("%s:\n", suite);
}

/*-------------------------------------------------------------------------*/

void parcourir_instr_faire(n_instr * n)
{				/* MODIFIE POUR EVAL *//* MODIFIE POUR EVAL */
    char *fct = "instr_faire";	/* MODIFIE POUR EVAL */
    //parcourir_balise_ouvrante(fct, trace_abs);    /* MODIFIE POUR EVAL */
    parcourir_instr(n->u.faire_.faire);	/* MODIFIE POUR EVAL */
    parcourir_exp(n->u.faire_.test);	/* MODIFIE POUR EVAL */
    //parcourir_balise_fermante(fct, trace_abs);    /* MODIFIE POUR EVAL */
}				/* MODIFIE POUR EVAL */

/*-------------------------------------------------------------------------*/

void parcourir_instr_pour(n_instr * n)
{				/* MODIFIE POUR EVAL *//* MODIFIE POUR EVAL */
    char *fct = "instr_pour";	/* MODIFIE POUR EVAL */
    //parcourir_balise_ouvrante(fct, trace_abs);         /* MODIFIE POUR EVAL */
    parcourir_instr(n->u.pour_.init);	/* MODIFIE POUR EVAL */
    parcourir_exp(n->u.pour_.test);	/* MODIFIE POUR EVAL */
    parcourir_instr(n->u.pour_.faire);	/* MODIFIE POUR EVAL */
    parcourir_instr(n->u.pour_.incr);	/* MODIFIE POUR EVAL */
    //parcourir_balise_fermante(fct, trace_abs);         /* MODIFIE POUR EVAL */
}				/* MODIFIE POUR EVAL */

/*-------------------------------------------------------------------------*/

void parcourir_instr_affect(n_instr * n)
{
    // validité de la variable est déjà verifiée
    int reg1 = parcourir_var_non_empiler(n->u.affecte_.var);
    int reg2 = parcourir_exp(n->u.affecte_.exp);

    //affiche_instr_affect(n);
    int i = rechercheExecutable(n->u.affecte_.var->nom);
    if (n->u.affecte_.var->type == simple) {
	// variable simple

	int reg3 = allouer_reg();

	//printf("la $t%d, %s\n",reg3,dico.tab[i].identif);
	char num = '0';
	char reg = 't';
	depiler_registre(reg, num);
	if (dico.tab[i].classe == C_VARIABLE_GLOBALE)
	    printf("\tsw $%c%c, %s\t\t# stocke dans variable globale\n",
		   reg, num, dico.tab[i].identif);
	if (dico.tab[i].classe == C_VARIABLE_LOCALE) {
	    int adresse = -8 - dico.tab[i].adresse;
	    printf
		("\tsw $%c%c, %d($fp)\t\t# stocke dans variable locale\n",
		 reg, num, adresse);
	}
	if (dico.tab[i].classe == C_ARGUMENT) {
	    int adresse = 4 * nb_argument() - dico.tab[i].adresse;
	    printf("\tsw $%c%c, %d($fp)\t\t# stocke dans argument\n", reg,
		   num, adresse);
	}
    } else {
	// tableau
	int adresse = dico.tab[i].adresse;
	int reg3 = allouer_reg();
	char reg = 't';
	char num1 = '1';
	char num0 = '0';
	depiler_registre(reg, num1);	// valeur d'expression à gauche

	depiler_registre(reg, num0);	// valeur d'indice
	printf("\tadd $%c%c, $%c%c, $%c%c\n", reg, num0, reg, num0, reg,
	       num0);
	printf("\tadd $%c%c, $%c%c, $%c%c\n", reg, num0, reg, num0, reg,
	       num0);
	printf("\tsw $%c%c, %s($%c%c)\t\t# stocke dans variable\n", reg,
	       num1, dico.tab[i].identif, reg, num0);
    }

}

/*-------------------------------------------------------------------------*/

void parcourir_instr_appel(n_instr * n)
{
    char *fct = "instr_appel";
    //parcourir_balise_ouvrante(fct, trace_abs);


    parcourir_appel(n->u.appel);
    //parcourir_balise_fermante(fct, trace_abs);
}

/*-------------------------------------------------------------------------*/

int parcourir_appel(n_appel * n)
{
    int nbArg = 0;
    int i = -1;
    printf
	("\tsubi $sp, $sp, 4\t# mot sur la pile pour valeur de retour\n");

    // parcourir_l_exp stocke les arguments sur la pile
    nbArg = parcourir_l_exp(n->args);

    i = rechercheExecutable(n->fonction);
    if (i == -1) {
	erreur("fonction n'est pas declarée");
    } else if (dico.tab[i].type != T_FONCTION) {
	erreur("identificateur n'est pas une fonction");
    } else if (dico.tab[i].complement != nbArg) {
	printf("n: fonction %s\n", n->fonction);
	printf("n: arg %d\n", nbArg);
	erreur("fonction %s ne satisfait pas le type necessaire");
    }

    printf("\tjal %s\t\t\t# appel fonction\n", n->fonction);
    int j;
    for (j = 0; j < nbArg; j++) {
	depiler_registre('t', '0');
    }
    // pour l'instant
    return -1;
}

/*-------------------------------------------------------------------------*/

void parcourir_instr_retour(n_instr * n)
{

    parcourir_exp(n->u.retour_.expression);


    if (n->u.retour_.expression) {
	int adresse = 4 * (nb_argument() + 1);
	char reg = 't';
	char num = '0';
	depiler_registre(reg, num);
	printf("\tsw $%c%c, %d($fp)\t\t# return\n", reg, num, adresse);
	printf("\tmove $sp, $fp\n");
	printf("\tla $sp, -4($fp)\n");
	depiler_registre('r', 'a');
	depiler_registre('f', 'p');
	printf("\tjr $ra\t\t# jump back\n");
    } else {
	int adresse = 4 * (nb_argument() + 1);
	printf("\tli $t0, 0\t\t\t# default return = 0\n");
	printf("\tsw $t0, %d($fp)\n", adresse);
	printf("\tmove $sp, $fp\n");
	printf("\tla $sp, -4($fp)\n");
	depiler_registre('r', 'a');
	depiler_registre('f', 'p');
	printf("\tjr $ra\t\t# jump back\n");
    }


}

/*-------------------------------------------------------------------------*/

void parcourir_instr_ecrire(n_instr * n)
{
    char *fct = "instr_ecrire";
    parcourir_exp(n->u.ecrire_.expression);
    char reg = 't';
    char num = '0';
    depiler_registre(reg, num);
    printf("\tli $v0, 1\t\t\n");
    printf("\tmove $a0, $%c%c\n", reg, num);
    printf("\tsyscall\t\t\t#imprimer registre $%c%c\n", reg, num);
}

/*-------------------------------------------------------------------------*/

int parcourir_l_exp(n_l_exp * n)
{
    int nombre = 1;
    if (n == NULL)
	return 0;
    if (n) {
	parcourir_exp(n->tete);
	nombre += parcourir_l_exp(n->queue);
    }
    return nombre;
}

/*-------------------------------------------------------------------------*/

int parcourir_exp(n_exp * n)
{
    int numero = -1;
    if (n->type == varExp)
	numero = parcourir_varExp(n);
    else if (n->type == opExp)
	numero = parcourir_opExp(n);
    else if (n->type == intExp)
	numero = parcourir_intExp(n);
    else if (n->type == appelExp)
	numero = parcourir_appelExp(n);
    else if (n->type == lireExp)
	numero = parcourir_lireExp(n);

    return numero;
}

/*-------------------------------------------------------------------------*/

int parcourir_varExp(n_exp * n)
{
    char *fct = "varExp";
    return parcourir_var(n->u.var);
}

/*-------------------------------------------------------------------------*/
int parcourir_opExp(n_exp * n)
{
    int numero1, numero2;
    if (n->u.opExp_.op1 != NULL) {
	numero1 = parcourir_exp(n->u.opExp_.op1);
    }
    if (n->u.opExp_.op2 != NULL) {
	numero2 = parcourir_exp(n->u.opExp_.op2);
    }
    if (n->u.opExp_.op == plus) {
	char reg = 't';
	depiler_registre(reg, '1');
	depiler_registre(reg, '0');

	printf("\tadd $%c%c, $%c%c, $%c%c\t\t# addition\n", reg, '0', reg,
	       '1', reg, '0');
	empiler_registre(reg, '0');
	return numero1;
    } else if (n->u.opExp_.op == moins) {
	char reg = 't';
	depiler_registre(reg, '1');
	depiler_registre(reg, '0');

	printf("\tsub $%c%c, $%c%c, $%c%c\t\t# soustraction\n", reg, '0',
	       reg, '0', reg, '1');
	empiler_registre(reg, '0');
	return numero1;
    } else if (n->u.opExp_.op == fois) {
	char reg = 't';
	depiler_registre(reg, '1');
	depiler_registre(reg, '0');

	printf("\tmult $%c%c, $%c%c\t\t# multiplication\n", reg, '0', reg,
	       '1');
	printf("\tmflo $%c%c\n", reg, '0');
	empiler_registre(reg, '0');
	return numero1;
    } else if (n->u.opExp_.op == divise) {
	char reg = 't';
	depiler_registre(reg, '1');
	depiler_registre(reg, '0');

	printf("\tdivide $%c%c, $%c%c\t\t# division\n", reg, '0', reg,
	       '1');
	printf("\tmflo $%c%c\n", reg, '0');
	empiler_registre(reg, '0');
	return numero1;
    }
	/***********************************************
	 *  VRAI == -1
	 *  FAUX == '0' 
	 *************************************************/
    else if (n->u.opExp_.op == egal) {
	char reg = 't';
	depiler_registre(reg, '1');
	depiler_registre(reg, '0');

	char s[20];
	char r[20];
	get_label("e", s);
	get_label("e", r);
	printf("\tbeq $%c%c, $%c%c, %s\t\t# tester de l'egalite\n", reg,
	       '0', reg, '1', s);
	printf("\tli $%c%c, 0\n", reg, '0');
	printf("\tj %s\n", r);
	printf("%s:\tli $%c%c, -1\n", s, reg, '0');
	printf("%s:\n", r);
	empiler_registre(reg, '0');
	return numero1;
    } else if (n->u.opExp_.op == diff) {
	char reg = 't';
	depiler_registre(reg, '1');
	depiler_registre(reg, '0');

	char s[20];
	char r[20];
	get_label("e", s);
	get_label("e", r);
	printf("\tbne $%c%c, $%c%c, %s\t\t# tester de l'inegalite\n", reg,
	       '0', reg, '1', s);
	printf("\tli $%c%c, 0\n", reg, '0');
	printf("\tj %s\n", r);
	printf("%s:\tli $%c%c, -1\n", s, reg, '0');
	printf("%s:\n", r);
	empiler_registre(reg, '0');
	return numero1;
    } else if (n->u.opExp_.op == inf) {
	char reg = 't';
	depiler_registre(reg, '1');
	depiler_registre(reg, '0');

	char s[20];
	char r[20];
	get_label("e", s);
	get_label("e", r);
	printf("\tblt $%c%c, $%c%c, %s\t\t# tester de <\n", reg, '0', reg,
	       '1', s);
	printf("\tli $%c%c, 0\n", reg, '0');
	printf("\tj %s\n", r);
	printf("%s:\tli $%c%c, -1\n", s, reg, '0');
	printf("%s:\n", r);
	empiler_registre(reg, '0');
	return numero1;
    } else if (n->u.opExp_.op == infeg) {
	char reg = 't';
	depiler_registre(reg, '1');
	depiler_registre(reg, '0');

	char s[20];
	char r[20];
	get_label("e", s);
	get_label("e", r);
	printf("\tble $%c%c, $%c%c, %s\t\t# tester de <=\n", reg, '0', reg,
	       '1', s);
	printf("\tli $%c%c, 0\n", reg, '0');
	printf("\tj %s\n", r);
	printf("%s:\tli $%c%c, -1\n", s, reg, '0');
	printf("%s:\n", r);
	empiler_registre(reg, '0');
	return numero1;
    } else if (n->u.opExp_.op == ou) {
	char reg = 't';
	depiler_registre(reg, '1');
	depiler_registre(reg, '0');

	char s[20];
	char r[20];
	get_label("e", s);
	get_label("e", r);
	printf("\tli $%c%c, 0\t\t# test de ||\n", reg, '2');
	printf("\tbne $%c%c, $%c%c, %s\n", reg, '0', reg, '2', s);
	printf("\tbne $%c%c, $%c%c, %s\n", reg, '1', reg, '2', s);
	printf("\tli $%c%c, 0\n", reg, '0');
	printf("\tj %s\n", r);
	printf("%s: li $%c%c -1\n", s, reg, '0');
	printf("%s:\n", r);
	empiler_registre(reg, '0');
	return numero1;
    } else if (n->u.opExp_.op == et) {
	char reg = 't';
	depiler_registre(reg, '1');
	depiler_registre(reg, '0');

	char s[20];
	char r[20];
	get_label("e", s);
	get_label("e", r);
	printf("\tli $%c%c, 0\t\t# test de &&\n", reg, '2');
	printf("\tbeq $%c%c, $%c%c, %s\n", reg, '0', reg, '2', s);
	printf("\tbeq $%c%c, $%c%c, %s\n", reg, '1', reg, '2', s);
	printf("\tli $%c%c, -1\n", reg, '0');
	printf("\tj %s\n", r);
	printf("%s: li $%c%c 0\n", s, reg, '0');
	printf("%s:\n", r);
	empiler_registre(reg, '0');
	return numero1;
    } else if (n->u.opExp_.op == non) {
	char reg = 't';
	depiler_registre(reg, '0');
	char s[20];
	char r[20];
	get_label("e", s);
	get_label("e", r);
	printf("\tli $%c%c, 0\t\t# ! operator\n", reg, '2');
	printf("\tbeq $%c%c, $%c%c, %s\n", reg, '0', reg, '2', s);
	printf("\tli $%c%c, 0\n", reg, '0');
	printf("\tj %s\n", r);
	printf("%s: li $%c%c -1\n", s, reg, '0');
	printf("%s:\n", r);
	empiler_registre(reg, '0');
	return numero1;
    }
}

/*-------------------------------------------------------------------------*/

int parcourir_intExp(n_exp * n)
{
    char texte[50];		// Max. 50 chiffres
    int reg = allouer_reg();
    sprintf(texte, "%d", n->u.entier);
    printf("\tli $t0, %s\t\t# lit directement entier dans registre\n",
	   texte);
    empiler_registre('t', '0');
    return reg;
}

/*-------------------------------------------------------------------------*/
int parcourir_lireExp(n_exp * n)
{

    int reg = allouer_reg();
    printf("\tli $v0, 5\n");
    printf("\tsyscall\t\t# lit valeur depuis utilisateur\n");
    empiler_registre('v', '0');
    return reg;

}

/*-------------------------------------------------------------------------*/

int parcourir_appelExp(n_exp * n)
{
    char *fct = "appelExp";
    return parcourir_appel(n->u.appel);

}

/*-------------------------------------------------------------------------*/

int parcourir_l_dec(n_l_dec * n)
{
    if (n == NULL)
	return 0;

    char *fct = "l_dec";
    int nombre = 1;
    if (n) {
	parcourir_dec(n->tete);
	nombre += parcourir_l_dec(n->queue);

    }
    return nombre;
}

/*-------------------------------------------------------------------------*/

void parcourir_dec(n_dec * n)
{

    if (n) {
	if (n->type == foncDec) {
	    parcourir_foncDec(n);
	} else if (n->type == varDec) {
	    parcourir_varDec(n);

	} else if (n->type == tabDec) {
	    parcourir_tabDec(n);
	}
    }
}

/*-------------------------------------------------------------------------*/

void parcourir_foncDec(n_dec * n)
{
    int i = rechercheExecutable(n->nom);
    if (i == -1) {
	printf("%s:\n", n->nom);
	i = ajouteIdentificateur(n->nom, C_VARIABLE_GLOBALE, T_FONCTION, 0,
				 0);
	entreeFonction();

	empiler_registre('f', 'p');
	printf("\tmove $fp, $sp\t\t# nouvelle valeur de $fp\n");
	empiler_registre('r', 'a');

	// set default value for the return value;

	contexte = C_ARGUMENT;
	int nombre = parcourir_l_dec(n->u.foncDec_.param);

	printf("#\t\t\t\t\t\t\t\t\t\t\t\t\t\tnb_argument() = %d\n",
	       nb_argument());
	int adresse = 4 * (nb_argument() + 1);
	printf("\tli $t0, 0\t\t# default return = 0\n");
	printf("\tsw $t0, %d($fp)\n", adresse);

	contexte = C_VARIABLE_LOCALE;
	int nbLocal = parcourir_l_dec(n->u.foncDec_.variables);
	printf("#\t\t\t\t\t\t\t\t\t\t\t\t\t\tnb_local = %d\n", nbLocal);
	nbLocal *= 4;

	printf
	    ("\tsubi $sp, $sp, %d\t# allouer de la place pour les variables locales\n",
	     nbLocal);
	dico.tab[i].complement = nombre;
	parcourir_instr(n->u.foncDec_.corps);
	
#if AFFICHE_DICO
	affiche_dico();
#endif

	printf
	    ("\taddi $sp, $sp, %d\t# deallouer de la place pour les variables locales\n",
	     nbLocal);
	printf("\tmove $sp, $fp\n");
	printf("\tla $sp, -4($fp)\n");
	depiler_registre('r', 'a');
	depiler_registre('f', 'p');
	printf("\tjr $ra\t\t# jump back\n");
	sortieFonction();
    } else {
	erreur("fonction déjà déclarée");
    }
}

/*-------------------------------------------------------------------------*/

void parcourir_varDec(n_dec * n)
{
    if (contexte == C_VARIABLE_GLOBALE) {
	int i = rechercheDeclarative(n->nom);
	if (i == -1) {
	    ajouteIdentificateur(n->nom, C_VARIABLE_GLOBALE, T_ENTIER,
				 adresseGlobalCourante, -1);
	    adresseGlobalCourante += 4;
	    printf("%s: .space 4\n", n->nom);
	} else {
	    erreur("Variable déjà declarée");
	}
    } else if (contexte == C_VARIABLE_LOCALE) {
	int i = rechercheDeclarative(n->nom);
	if (i == -1) {
	    ajouteIdentificateur(n->nom, C_VARIABLE_LOCALE, T_ENTIER,
				 adresseLocaleCourante, -1);
	    adresseLocaleCourante += 4;
	} else {
	    erreur("Variable déjà declarée");
	}
    } else {
	int i = rechercheDeclarative(n->nom);
	if (i == -1) {
	    ajouteIdentificateur(n->nom, C_ARGUMENT, T_ENTIER,
				 adresseArgumentCourant, -1);
	    adresseArgumentCourant += 4;
	} else {
	    erreur("Nom d'argument utilisée");
	}
    }
}

/*-------------------------------------------------------------------------*/

void parcourir_tabDec(n_dec * n)
{
    if (contexte == C_VARIABLE_GLOBALE) {
	int i = rechercheDeclarative(n->nom);
	if (i == -1) {
	    ajouteIdentificateur(n->nom, C_VARIABLE_GLOBALE,
				 T_TABLEAU_ENTIER, adresseGlobalCourante,
				 n->u.tabDec_.taille);
	    adresseGlobalCourante += 4;
	    printf("%s: .space %d\n", n->nom, n->u.tabDec_.taille * 4);
	} else {
	    erreur("Variable déjà declarée");
	}
    }
}

/*-------------------------------------------------------------------------*/

int parcourir_var(n_var * n)
{

    if (n->type == simple) {
	return parcourir_var_simple(n, 1);
    } else if (n->type == indicee) {
	return parcourir_var_indicee(n, 1);
    }

}

/** declaré par moi */

int parcourir_var_non_empiler(n_var * n)
{

    if (n->type == simple) {
	return parcourir_var_simple(n, 0);
    } else if (n->type == indicee) {
	return parcourir_var_indicee(n, 0);
    }
}

/*-------------------------------------------------------------------------*/
int parcourir_var_simple(n_var * n, int empiler_Valeur)
{
    int i = rechercheExecutable(n->nom);
    if (i == -1)
	erreur("variable n'est pas déclarée");
    if (dico.tab[i].type != T_ENTIER) {
	erreur("nom utilisée n'est pas une variable de type entier");
    }

    if (empiler_Valeur) {
	char num = '0';
	char reg = 't';
	if (dico.tab[i].classe == C_VARIABLE_GLOBALE) {
	    printf
		("\tlw $%c%c, %s\t\t# lit variable globale dans registre\n",
		 reg, num, dico.tab[i].identif);
	    empiler_registre(reg, num);
	}
	if (dico.tab[i].classe == C_VARIABLE_LOCALE) {
	    int adresse = -8 - dico.tab[i].adresse;
	    printf
		("\tlw $%c%c, %d($fp)\t\t # lit variable locale dans registre\n",
		 reg, num, adresse);
	    empiler_registre(reg, num);
	}
	if (dico.tab[i].classe == C_ARGUMENT) {
	    int adresse = 4 * nb_argument() - dico.tab[i].adresse;
	    printf
		("\tlw $%c%c, %d($fp)\t\t # lit argument dans registre\n",
		 reg, num, adresse);
	    empiler_registre(reg, num);
	}
    }
    return 1;
}

/*-------------------------------------------------------------------------*/
int parcourir_var_indicee(n_var * n, int empiler_Valeur)
{
    int i = rechercheExecutable(n->nom);
    if (i >= 0) {
	if (dico.tab[i].type != T_TABLEAU_ENTIER)
	    erreur
		("nom utilisée n'est pas une variable de type tableau entier");
    } else
	erreur("nom utilisée n'est pas declarée");

    int indice = parcourir_exp(n->u.indicee_.indice);
    int reg1 = allouer_reg();	// $t1
    int reg2 = allouer_reg();	// $t2

    if (empiler_Valeur) {
	char reg = 't';
	char num = '0';
	depiler_registre(reg, num);
	printf("\tadd $%c%c, $%c%c, $%c%c\n", reg, num, reg, num, reg,
	       num);
	printf("\tadd $%c%c, $%c%c, $%c%c\n", reg, num, reg, num, reg,
	       num);
	printf("\tlw $%c%c, %s($%c%c)\n", reg, num, dico.tab[i].identif,
	       reg, num);
	empiler_registre(reg, num);
    }
    return reg1;
}

/*-------------------------------------------------------------------------*/
