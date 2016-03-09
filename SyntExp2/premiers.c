#include"symboles.h"
#include"premiers.h"

#if 0
void initialise_premiers(void){
  int i,j;
  
  // Initialiser toutes les cases du tableau à 0
  for(i=0; i <= NB_NON_TERMINAUX; i++)
    for(j=0; j <= NB_TERMINAUX; j++)
      premiers[i][j] = 0;
  
  // Exemple
  premiers[_programme_][ENTIER] = 1; 
  
}
#endif

int premiers[NB_NON_TERMINAUX+1][NB_TERMINAUX+1] = {
	[_programme_] = {
		[ENTIER] = 1,
		[ID_FCT] = 1,
		[EPSILON] = 1
	},
	[_optDecVariables_] = {
		[EPSILON] = 1,
		[ENTIER] = 1
	},
	[_listeDecVariables_][ENTIER] = 1,
	[_listeDecVariablesBis_] = {
		[EPSILON] = 1,
		[VIRGULE] = 1
	},
	[_declarationVariable_][ENTIER] = 1,
	[_optTailleTableau_] = {
		[EPSILON] = 1,
		[CROCHET_OUVRANT] = 1
	},
	[_listeDecFonctions_] = {
		[EPSILON] = 1,
		[ID_FCT] = 1
	},
	[_declarationFonction_][ID_FCT] = 1,
	[_listeParam_][PARENTHESE_OUVRANTE] = 1,
	[_optListeDecVariables_] = {
		[EPSILON] = 1,
		[ENTIER] = 1
	},	
	[_instruction_] = {
		[ID_VAR] = 1,
		[ACCOLADE_OUVRANTE] = 1,
		[SI] = 1,
		[TANTQUE] = 1,
		[ID_FCT] = 1,
		[RETOUR] = 1,
		[ECRIRE] = 1,
		[POINT_VIRGULE] = 1
	},
	[_instructionAffect_][ID_VAR] = 1,
	[_instructionBloc_][ACCOLADE_OUVRANTE] = 1,
	[_listeInstructions_] = {
		[EPSILON] = 1,
		[ID_VAR] = 1,
		[ACCOLADE_OUVRANTE] = 1,
		[SI] = 1,
		[TANTQUE] = 1,
		[ID_FCT] = 1,
		[RETOUR] = 1,
		[ECRIRE] = 1,
		[POINT_VIRGULE] = 1
	},
	[_instructionSi_][SI] = 1,
	[_optSinon_] = {
		[SINON] = 1,
		[EPSILON] = 1
	},
	[_instructionTantque_][TANTQUE] = 1,
	[_instructionAppel_][ID_FCT] = 1,
	[_instructionRetour_][RETOUR] = 1,
	[_instructionEcriture_][ECRIRE] = 1,
	[_instructionVide_][POINT_VIRGULE] = 1,
	[_expression_] = {
		[NON] = 1,
		[PARENTHESE_OUVRANTE] = 1,
		[NOMBRE] = 1,
		[ID_FCT] = 1,
		[ID_VAR] = 1,
		[LIRE] = 1
	},
	[_expressionBis_] = {
		[OU] = 1,
		[EPSILON] = 1
	},
	[_conjonction_] = {
		[NON] = 1,
		[PARENTHESE_OUVRANTE] = 1,
		[NOMBRE] = 1,
		[ID_FCT] = 1,
		[ID_VAR] = 1,
		[LIRE] = 1
	},
	[_conjonctionBis_] = {
		[ET] = 1,
		[EPSILON] = 1
	},
	[_negation_] = {
		[NON] = 1,
		[PARENTHESE_OUVRANTE] = 1,
		[NOMBRE] = 1,
		[ID_FCT] = 1,
		[ID_VAR] = 1,
		[LIRE] = 1
	},
	[_comparaison_] = {
		[PARENTHESE_OUVRANTE] = 1,
		[NOMBRE] = 1,
		[ID_FCT] = 1,
		[ID_VAR] = 1,
		[LIRE] = 1
	},
	[_comparaisonBis_] = {
		[EGAL] = 1,
		[INFERIEUR] = 1,
		[EPSILON] = 1
	},
	[_expArith_] = {
		[PARENTHESE_OUVRANTE] = 1,
		[NOMBRE] = 1,
		[ID_FCT] = 1,
		[ID_VAR] = 1,
		[LIRE] = 1
	},
	[_expArithBis_] = {
		[PLUS] = 1,
		[MOINS] = 1,
		[EPSILON] = 1	
	},
	[_terme_] = {
		[PARENTHESE_OUVRANTE] = 1,
		[NOMBRE] = 1,
		[ID_FCT] = 1,
		[ID_VAR] = 1,
		[LIRE] = 1
	},
	[_termeBis_] = {
		[FOIS] = 1,
		[DIVISE] = 1,
		[EPSILON] = 1
	},
	[_facteur_] = {
		[PARENTHESE_OUVRANTE] = 1,
		[NOMBRE] = 1,
		[ID_FCT] = 1,
		[ID_VAR] = 1,
		[LIRE] = 1
	},
	[_var_][ID_VAR] = 1,
	[_optIndice_] = {
		[CROCHET_OUVRANT] = 1,
		[EPSILON] = 1
	},
	[_appelFct_][ID_FCT] = 1,
	[_listeExpressions_] = {
		[NON] = 1,
		[PARENTHESE_OUVRANTE] = 1,
		[NOMBRE] = 1,
		[ID_FCT] = 1,
		[ID_VAR] = 1,
		[LIRE] = 1,
		[EPSILON] = 1
	},
	[_listeExpressionsBis_][VIRGULE] = 1,
	[_listeDecVariablesBis_][EPSILON] = 1
	
};

int est_premier(int non_terminal, int terminal)
{
	if (non_terminal < NB_NON_TERMINAUX && terminal < NB_TERMINAUX)
		return premiers[non_terminal][terminal];
	return 0;
}
