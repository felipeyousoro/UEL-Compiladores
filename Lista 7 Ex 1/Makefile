all: lex.yy.c
	gcc -o l7e1 lex.yy.c -ll

lex.yy.c: l7e1.l
	flex l7e1.l

clean:
	rm -rf l5e2
    