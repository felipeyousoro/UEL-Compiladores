all: lex.yy.c
	gcc -o l7e2 lex.yy.c -ll

lex.yy.c: l7e2.l
	flex l7e2.l

clean:
	rm -rf l7e2
    