%option noyywrap
%option yylineno

%{
    #include <sintatico.tab.h>

%}

%%

[ \t]                       { }
[\n]                        { return NEWLINE; }
"Grafo"                     { return GRAPH; }
"-->"                       { return ARROW; }
":"                         { return COLON; }
"="                         { return ASSIGN; }
"K"                         { return K; }
[0-9]+                      { yylval.integer = atoi(yytext); return NUMBER; }

.                           { printf("UNKNOWN\n"); }

<<EOF>>                     { return END_OF_FILE; }

%%
