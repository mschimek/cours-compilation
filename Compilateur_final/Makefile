CC = gcc

LIBS = -lm 
CCFLAGS = -Wall -ggdb3 -std=gnu11

OBJ = analyseur_lexical.o util.o premiers.o suivants.o affiche_arbre_abstrait.o syntabs.o

OBJ2 = analyseur_lexical.o util.o premiers.o suivants.o  syntabs.o  dico.o  parcours_arbre.o

all: analyseur


analyseur: analyseur.c $(OBJ2)
	$(CC) $(CCFLAGS) -o analyseur analyseur.c $(OBJ2)

analyseur_arbre_abstrait: analyseur_arbre_abstrait.c $(OBJ)
	$(CC) $(CCFLAGS) -o analyseur_arbre_abstrait analyseur_arbre_abstrait.c $(OBJ)

analyseur_syntaxique_LL: analyseur_syntaxique_LL.c $(OBJ)
	$(CC) $(CCFLAGS) -o analyseur_syntaxique_LL analyseur_syntaxique_LL.c $(OBJ)


analyseur_lexical.o: analyseur_lexical.c
	$(CC) $(CCFLAGS) -c $^

test_yylex: test_yylex.c $(OBJ)
	$(CC) $(CCFLAGS) -o test_yylex test_yylex.c $(OBJ)


.PHONY : clean

clean:
	- rm -f $(OBJ)
	- rm -f $(OBJ2)
	- rm -f analyseur_lexical
	- rm -f test_yylex
	- rm -f analyseur_syntaxique_LL
	- rm -f analyseur_arbre_abstrait
	- rm -f analyseur