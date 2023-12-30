CC = gcc
CFLAGS = -std=c99
BISONFLAGS = -d -t
EXEFLAGS = -I. -DYYDEBUG=1

SINTATICO = sintatico.tab.c 
LEXICO = lex.yy.c

TESTES = $(wildcard Entradas/*.txt)
TESTES := $(TESTES:Entradas/%=%)

MAIN = main.c

SRC = graph.c register.c list.c stack.c
OBJ = $(SRC:.c=.o)

all: gerador clear test

iniciar:
	@clear
	@echo
	@echo
	@echo
	@echo "***********************************************************"
	@echo "***********************************************************"
	@echo "******************                         ****************"
	@echo "**************** Alocador de Registradores  ***************"
	@echo "******************                         ****************"
	@echo "***********************************************************"
	@echo "***********************************************************"
	@echo
	@echo "By: Felipe Yousoro"
	@echo "Disciplina: Compiladores"
	@echo "Data: 24/04/2023"
	@echo
	@echo "******************** Compilando arquivos e objetos... ********************"
	@echo

gerador: $(LEXICO) $(SINTATICO) $(OBJ)
	@echo
	@echo "******************** Gerando executável... ********************"
	@echo
	$(CC) $(OBJ) $(LEXICO) $(SINTATICO) $(MAIN) $(EXEFLAGS) $(CFLAGS) -o regalloc

test:
	@sleep 2
	@echo
	@echo "******************** Testando... ********************"
	@echo
	for test in $(TESTES); do ./regalloc < Entradas/$$test > Saidas/$$test; done


	
diff:
	@sleep 4
	@echo
	@echo "******************** Checando diffs... ********************"
	@echo
	for test in $(TESTES); do diff -b Saidas/$$test Respostas/saida_$$test; done


$(LEXICO): lexico.l
	flex lexico.l 

$(SINTATICO): sintatico.y
	bison $(BISONFLAGS) sintatico.y
	
%.o: %.c
	$(CC) -c $< -o $@

clear:
	@sleep 4
	@echo
	@echo "******************** Limpando arquivos temporários... ********************"
	@echo
	rm -f *.o lex.yy.c sintatico.tab.c sintatico.tab.h $(OBJ)