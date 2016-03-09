CC = gcc

LIBS = -lm 
CCFLAGS = -Wall -ggdb3 -std=gnu11

OBJ = analyseur_lexical.o util.o premiers.o suivants.o

all: analyseur_syntaxique_LL

analyseur_syntaxique_LL: analyseur_syntaxique_LL.c $(OBJ)
	$(CC) $(CCFLAGS) -o analyseur_syntaxique_LL analyseur_syntaxique_LL.c $(OBJ)


analyseur_lexical.o: analyseur_lexical.c
	$(CC) $(CCFLAGS) -c $^

test_yylex: test_yylex.c $(OBJ)
	$(CC) $(CCFLAGS) -o test_yylex test_yylex.c $(OBJ)


.PHONY : clean

clean:
	- rm -f $(OBJ)
	- rm -f analyseur_lexical	
	- rm -f analyseur_syntaxique_LL