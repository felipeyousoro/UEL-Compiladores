CC = gcc
CFLAGS = -std=c99
BISONFLAGS = -d -t #--report-file=Saidas/sintatico.report --report=all
EXEFLAGS = -I. #-DYYDEBUG=1
VALGRIND = valgrind #--leak-check=full --show-leak-kinds=all --track-origins=yes

STACK = data_structures/stack/stack.c
SYMBOL_TABLE = data_structures/symbol_table/symbol_table.c
LIST = data_structures/list/list.c
DATA_STRUCTURES = $(STACK) $(SYMBOL_TABLE) $(LIST)
AST_NODES = $(wildcard ast_nodes/*/*.c)
CODE_GEN = code_generator.c
CTXT_MNGER = context_manager.c
MACROS = macros.c

SINTATICO = sintatico.tab.c 
LEXICO = lex.yy.c

MAIN = main.c

SRC = $(DATA_STRUCTURES) $(AST_NODES) $(CODE_GEN) $(CTXT_MNGER) $(MACROS)
OBJ = $(SRC:.c=.o)

all: iniciar gerador clear test

iniciar:
	@clear
	@echo
	@echo
	@echo
	@echo "***********************************************************"
	@echo "***********************************************************"
	@echo "********************                   ********************"
	@echo "******************** Gerador de Código ********************"
	@echo "********************                   ********************"
	@echo "***********************************************************"
	@echo "***********************************************************"
	@echo
	@echo "By: Felipe Yousoro"
	@echo "Disciplina: Compiladores"
	@echo "Data: 04/04/2023"
	@echo
	@echo "******************** Compilando arquivos e objetos... ********************"
	@echo

gerador: $(LEXICO) $(SINTATICO) $(OBJ)
	@echo
	@echo "******************** Gerando executável... ********************"
	@echo
	gcc $(OBJ) $(LEXICO) $(SINTATICO) $(MAIN) $(EXEFLAGS) -o gerador

test:
	@sleep 2
	@echo
	@echo "******************** Testando... ********************"
	@echo
	$(VALGRIND) ./gerador < Entradas/testebaseado.txt > Saidas/testebaseado.txt -g "Saidas/testebaseado.dot"
	./gerador < Entradas/triangulo.txt > Saidas/triangulo.txt -g "Saidas/triangulo.dot"
	./gerador < Entradas/fatorial.txt > Saidas/fatorial.txt -g "Saidas/fatorial.dot"
	./gerador < Entradas/comparacao.txt > Saidas/comparacao.txt -g "Saidas/comparacao.dot"
	./gerador < Entradas/quicksort.txt > Saidas/quicksort.txt -g "Saidas/quicksort.dot"
	./gerador < Entradas/ELDIABO.txt > Saidas/ELDIABO.txt -g "Saidas/ELDIABO.dot"
	
diff:
	@echo
	@echo "******************** Checando diffs... ********************"
	@echo
	diff Saidas/testebaseado.txt Saidas/saida_vazia.txt
	diff Saidas/triangulo.txt Saidas/saida_vazia.txt
	diff Saidas/fatorial.txt Saidas/saida_vazia.txt
	diff Saidas/comparacao.txt Saidas/saida_vazia.txt
	diff Saidas/quicksort.txt Saidas/saida_vazia.txt
	diff Saidas/ELDIABO.txt Saidas/saida_vazia.txt

graphviz:
	@sleep 3
	@echo
	@echo "******************** Gerando gráficos... ********************"
	@echo
	@# dot Saidas/testebaseado.dot | gvpr -c -f Entradas/tree.gv | neato -n -Tsvg -o Saidas/testebaseado.svg
	@# dot Saidas/triangulo.dot | gvpr -c -f Entradas/tree.gv | neato -n -Tsvg -o Saidas/triangulo.svg
	@# dot Saidas/fatorial.dot | gvpr -c -f Entradas/tree.gv | neato -n -Tsvg -o Saidas/fatorial.svg
	@# dot Saidas/comparacao.dot | gvpr -c -f Entradas/tree.gv | neato -n -Tsvg-o Saidas/comparacao.svg
	@# dot Saidas/quicksort.dot | gvpr -c -f Entradas/tree.gv | neato -n -Tsvg -o Saidas/quicksort.svg
	@# dot Saidas/ELDIABO.dot | gvpr -c -f Entradas/tree.gv | neato -n -Tsvg -o Saidas/ELDIABO.svg
	dot -Tsvg Saidas/testebaseado.dot -o Saidas/testebaseado.svg
	dot -Tsvg Saidas/triangulo.dot -o Saidas/triangulo.svg
	dot -Tsvg Saidas/fatorial.dot -o Saidas/fatorial.svg
	@#dot -Tsvg Saidas/comparacao.dot -o Saidas/comparacao.svg
	dot -Tsvg Saidas/quicksort.dot -o Saidas/quicksort.svg
	dot -Tsvg Saidas/ELDIABO.dot -o Saidas/ELDIABO.svg

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
	rm -f *.o lex.yy.c sintatico.tab.c $(OBJ)