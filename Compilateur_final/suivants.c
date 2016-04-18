#include <stdio.h>
#include <stdlib.h>
#include"symboles.h"
#include"suivants.h"
#if 0
void initialise_suivants(void){
  int i,j;
  
  // Initialiser toutes les cases du tableau à 0
  for(i=0; i <= NB_NON_TERMINAUX; i++)
    for(j=0; j <= NB_TERMINAUX; j++)
      suivants[i][j] = 0;
      
  //exemple      
  suivants[_programme_][FIN] = 1;
  
  // TODO: Compléter le tableau suivants ici
}
#endif

int suivants[NB_NON_TERMINAUX+1][NB_TERMINAUX+1] = {
	[_programme_][FIN] = 1,
	[_optDecVariables_] = {
		[ID_FCT] = 1,
		[FIN] = 1,
		[ACCOLADE_OUVRANTE] = 1
	},
	[_listeDecVariables_] = {
		[POINT_VIRGULE] = 1,
		[PARENTHESE_FERMANTE] = 1
	},
	[_listeDecVariablesBis_] = {
		[POINT_VIRGULE] = 1,
		[PARENTHESE_FERMANTE] = 1
	},
	[_declarationVariable_] = {
		[VIRGULE] = 1,
		[POINT_VIRGULE] = 1,
		[PARENTHESE_FERMANTE] = 1
	},
	[_optTailleTableau_] = {
		[VIRGULE] = 1,
		[POINT_VIRGULE] = 1,
		[PARENTHESE_FERMANTE] = 1
	},
	[_listeDecFonctions_][FIN] = 1,
	[_declarationFonction_] = {
		[ID_FCT] = 1,
		[FIN] = 1
	},
	[_listeParam_] = {
		[ENTIER] = 1,
		[ACCOLADE_OUVRANTE] = 1
	},
	[_optListeDecVariables_][PARENTHESE_FERMANTE] = 1,
	[_instruction_] = {
		[ID_VAR] = 1,
		[ACCOLADE_OUVRANTE] = 1,
		[SI] = 1,
		[TANTQUE] = 1,
		[ID_FCT] = 1,
		[RETOUR] = 1,
		[ECRIRE] = 1, [ECRIREC] = 1,
		[POINT_VIRGULE] = 1,
		[ACCOLADE_FERMANTE] = 1
	},
	[_instructionAffect_] = {
		[ID_VAR] = 1,
		[ACCOLADE_OUVRANTE] = 1,
		[SI] = 1,
		[TANTQUE] = 1,
		[ID_FCT] = 1,
		[RETOUR] = 1,
		[ECRIRE] = 1, [ECRIREC] = 1,
		[POINT_VIRGULE] = 1,
		[ACCOLADE_FERMANTE] = 1
	},
	[_instructionBloc_] = {
		[ID_VAR] = 1,
		[ACCOLADE_OUVRANTE] = 1,
		[SI] = 1,
		[TANTQUE] = 1,
		[ID_FCT] = 1,
		[RETOUR] = 1,
		[ECRIRE] = 1, [ECRIREC] = 1,
		[POINT_VIRGULE] = 1,
		[ACCOLADE_FERMANTE] = 1,
		[FIN] = 1,
		[SINON] = 1
	},
	[_listeInstructions_][ACCOLADE_FERMANTE] = 1,
	[_instructionSi_] = {
		[ID_VAR] = 1,
		[ACCOLADE_OUVRANTE] = 1,
		[SI] = 1,
		[TANTQUE] = 1,
		[ID_FCT] = 1,
		[RETOUR] = 1,
		[ECRIRE] = 1, [ECRIREC] = 1,
		[POINT_VIRGULE] = 1,
		[ACCOLADE_FERMANTE] = 1
	},
	[_optSinon_] = {
		[ID_VAR] = 1,
		[ACCOLADE_OUVRANTE] = 1,
		[SI] = 1,
		[TANTQUE] = 1,
		[ID_FCT] = 1,
		[RETOUR] = 1,
		[ECRIRE] = 1, [ECRIREC] = 1,
		[POINT_VIRGULE] = 1,
		[ACCOLADE_FERMANTE] = 1
	},
	[_instructionTantque_] = {
		[ID_VAR] = 1,
		[ACCOLADE_OUVRANTE] = 1,
		[SI] = 1,
		[TANTQUE] = 1,
		[ID_FCT] = 1,
		[RETOUR] = 1,
		[ECRIRE] = 1, [ECRIREC] = 1,
		[POINT_VIRGULE] = 1,
		[ACCOLADE_FERMANTE] = 1
	},
	[_instructionAppel_] = {
		[ID_VAR] = 1,
		[ACCOLADE_OUVRANTE] = 1,
		[SI] = 1,
		[TANTQUE] = 1,
		[ID_FCT] = 1,
		[RETOUR] = 1,
		[ECRIRE] = 1, [ECRIREC] = 1,
		[POINT_VIRGULE] = 1,
		[ACCOLADE_FERMANTE] = 1
	},
	[_instructionEcriture_] = {
		[ID_VAR] = 1,
		[ACCOLADE_OUVRANTE] = 1,
		[SI] = 1,
		[TANTQUE] = 1,
		[ID_FCT] = 1,
		[RETOUR] = 1,
		[ECRIRE] = 1, [ECRIREC] = 1,
		[POINT_VIRGULE] = 1,
		[ACCOLADE_FERMANTE] = 1
	},
	[_instructionEcriturec_] = {
		[ID_VAR] = 1,
		[ACCOLADE_OUVRANTE] = 1,
		[SI] = 1,
		[TANTQUE] = 1,
		[ID_FCT] = 1,
		[RETOUR] = 1,
		[ECRIRE] = 1, [ECRIREC] = 1,
		[POINT_VIRGULE] = 1,
		[ACCOLADE_FERMANTE] = 1
	},
	[_instructionRetour_] = {
		[ID_VAR] = 1,
		[ACCOLADE_OUVRANTE] = 1,
		[SI] = 1,
		[TANTQUE] = 1,
		[ID_FCT] = 1,
		[RETOUR] = 1,
		[ECRIRE] = 1, [ECRIREC] = 1,
		[POINT_VIRGULE] = 1,
		[ACCOLADE_FERMANTE] = 1
	},
	[_instructionVide_] = {
		[ID_VAR] = 1,
		[ACCOLADE_OUVRANTE] = 1,
		[SI] = 1,
		[TANTQUE] = 1,
		[ID_FCT] = 1,
		[RETOUR] = 1,
		[ECRIRE] = 1, [ECRIREC] = 1, [ECRIREC] = 1,
		[POINT_VIRGULE] = 1,
		[ACCOLADE_FERMANTE] = 1
	},
	[_expression_] = {
		[POINT_VIRGULE] = 1,
		[ALORS] = 1,
		[FAIRE] = 1,
		[PARENTHESE_FERMANTE] = 1,
		[CROCHET_FERMANT] = 1,
		[VIRGULE] = 1
	},
	[_expressionBis_] = {
		[POINT_VIRGULE] = 1,
		[ALORS] = 1,
		[FAIRE] = 1,
		[PARENTHESE_FERMANTE] = 1,
		[CROCHET_FERMANT] = 1,
		[VIRGULE] = 1
	},
	[_conjonction_] = {
		[POINT_VIRGULE] = 1,
		[ALORS] = 1,
		[FAIRE] = 1,
		[PARENTHESE_FERMANTE] = 1,
		[CROCHET_FERMANT] = 1,
		[VIRGULE] = 1,
		[OU] = 1
	},
	[_conjonctionBis_] = {
		[POINT_VIRGULE] = 1,
		[ALORS] = 1,
		[FAIRE] = 1,
		[PARENTHESE_FERMANTE] = 1,
		[CROCHET_FERMANT] = 1,
		[VIRGULE] = 1,
		[OU] = 1
	},
	[_negation_] = {
		[POINT_VIRGULE] = 1,
		[ALORS] = 1,
		[FAIRE] = 1,
		[PARENTHESE_FERMANTE] = 1,
		[CROCHET_FERMANT] = 1,
		[VIRGULE] = 1,
		[OU] = 1,
		[ET] = 1
	},
	[_comparaison_] = {
		[POINT_VIRGULE] = 1,
		[ALORS] = 1,
		[FAIRE] = 1,
		[PARENTHESE_FERMANTE] = 1,
		[CROCHET_FERMANT] = 1,
		[VIRGULE] = 1,
		[OU] = 1,
		[ET] = 1
	},
	[_comparaisonBis_] = {
		[POINT_VIRGULE] = 1,
		[ALORS] = 1,
		[FAIRE] = 1,
		[PARENTHESE_FERMANTE] = 1,
		[CROCHET_FERMANT] = 1,
		[VIRGULE] = 1,
		[OU] = 1,
		[ET] = 1
	},
	[_expArith_] = {
		[POINT_VIRGULE] = 1,
		[ALORS] = 1,
		[FAIRE] = 1,
		[PARENTHESE_FERMANTE] = 1,
		[CROCHET_FERMANT] = 1,
		[VIRGULE] = 1,
		[OU] = 1,
		[ET] = 1,
		[EGAL] = 1,
		[INFERIEUR] = 1
	},
	[_expArithBis_] = {
		[POINT_VIRGULE] = 1,
		[ALORS] = 1,
		[FAIRE] = 1,
		[PARENTHESE_FERMANTE] = 1,
		[CROCHET_FERMANT] = 1,
		[VIRGULE] = 1,
		[OU] = 1,
		[ET] = 1,
		[EGAL] = 1,
		[INFERIEUR] = 1
	},
	[_terme_] = {
		[POINT_VIRGULE] = 1,
		[ALORS] = 1,
		[FAIRE] = 1,
		[PARENTHESE_FERMANTE] = 1,
		[CROCHET_FERMANT] = 1,
		[VIRGULE] = 1,
		[OU] = 1,
		[ET] = 1,
		[EGAL] = 1,
		[INFERIEUR] = 1,
		[PLUS] = 1,
		[MOINS] = 1
	},
	[_termeBis_] = {
		[POINT_VIRGULE] = 1,
		[ALORS] = 1,
		[FAIRE] = 1,
		[PARENTHESE_FERMANTE] = 1,
		[CROCHET_FERMANT] = 1,
		[VIRGULE] = 1,
		[OU] = 1,
		[ET] = 1,
		[EGAL] = 1,
		[INFERIEUR] = 1,
		[PLUS] = 1,
		[MOINS] = 1
	},
	[_facteur_] = {
		[POINT_VIRGULE] = 1,
		[ALORS] = 1,
		[FAIRE] = 1,
		[PARENTHESE_FERMANTE] = 1,
		[CROCHET_FERMANT] = 1,
		[VIRGULE] = 1,
		[OU] = 1,
		[ET] = 1,
		[EGAL] = 1,
		[INFERIEUR] = 1,
		[FOIS] = 1,
		[DIVISE] = 1,
		[PLUS] = 1,
		[MOINS] = 1
	},
	[_var_] = {
		[POINT_VIRGULE] = 1,
		[ALORS] = 1,
		[FAIRE] = 1,
		[PARENTHESE_FERMANTE] = 1,
		[CROCHET_FERMANT] = 1,
		[VIRGULE] = 1,
		[OU] = 1,
		[ET] = 1,
		[EGAL] = 1,
		[INFERIEUR] = 1,
		[FOIS] = 1,
		[DIVISE] = 1,
		[PLUS] = 1,
		[MOINS] = 1
	},
	[_optIndice_] = {
		[POINT_VIRGULE] = 1,
		[ALORS] = 1,
		[FAIRE] = 1,
		[PARENTHESE_FERMANTE] = 1,
		[CROCHET_FERMANT] = 1,
		[VIRGULE] = 1,
		[OU] = 1,
		[ET] = 1,
		[EGAL] = 1,
		[INFERIEUR] = 1,
		[FOIS] = 1,
		[DIVISE] = 1,
		[PLUS] = 1,
		[MOINS] = 1
	},
	[_appelFct_] = {
		[POINT_VIRGULE] = 1,
		[ALORS] = 1,
		[FAIRE] = 1,
		[PARENTHESE_FERMANTE] = 1,
		[CROCHET_FERMANT] = 1,
		[VIRGULE] = 1,
		[OU] = 1,
		[ET] = 1,
		[EGAL] = 1,
		[INFERIEUR] = 1,
		[FOIS] = 1,
		[DIVISE] = 1,
		[PLUS] = 1,
		[MOINS] = 1
	},
	[_listeExpressions_][PARENTHESE_FERMANTE] = 1,
	[_listeExpressionsBis_][PARENTHESE_FERMANTE] = 1
};

int est_suivant(int non_terminal, int terminal)
{
	if (non_terminal < NB_NON_TERMINAUX && terminal < NB_TERMINAUX)
		return suivants[non_terminal][terminal];
	return 0;
}
