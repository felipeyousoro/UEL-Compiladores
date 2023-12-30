%option noyywrap
%option yylineno

%{
    #include <sintatico.tab.h>
    #include <string.h>
    extern YYSTYPE yylval;

    //#define PRINT_TOKEN(token) {printf("%s%s", nextline, token); nextline[0] = '\n';}
    #define PRINT_TOKEN(token) { }
    #define PRINT_STRING()     { printf("%sSTRING: %s", nextline, yytext); nextline[0] = '\n'; }

%}

%%
[ \t]                       { /* ignorar espacos em branco */ }
[\n]                        { return NEWLINE; }
"AST -->"                   { return START; } 

FUNCTION                    { return FUNCTION; }
RETURN_TYPE                 { return RETURN_TYPE; }
VARIABLE                    { return VARIABLE; }
PARAMETER                   { return PARAMETER; }
VALUE                       { return VALUE; }
TYPE                        { return TYPE; }
PRINTF                      { return PRINTF; }
SCANF                       { return SCANF; }
RETURN                      { return RETURN; }
END_FUNCTION                { return END_FUNCTION; }
IF                          { return IF; }
CONSTANT                    { return CONSTANT; }
"GLOBAL VARIABLE"           { return GLOBAL_VARIABLE; }
WHILE                       { return WHILE; }
DO_WHILE                    { return DO_WHILE; }
FOR                         { return FOR; }
EXIT                        { return EXIT; }


void                        { return VOID; }
int                         { return INT; }
char                        { return CHAR; }

"+"                         { return PLUS; }
"-"                         { return MINUS; }
"*"                         { return MULTIPLY; }
"/"                         { return DIV; }
"%"                         { return REMAINDER; }
"++"                        { return INC; }
"--"                        { return DEC; }
"&"                         { return BITWISE_AND; }
"|"                         { return BITWISE_OR; }
"~"                         { return BITWISE_NOT; }
"^"                         { return BITWISE_XOR; }
"!"                         { return LOGICAL_NOT; }
"&&"                        { return LOGICAL_AND; }
"||"                        { return LOGICAL_OR; }
"=="                        { return EQUAL; }
"!="                        { return NOT_EQUAL; }
"<"                         { return LESS_THAN; }
">"                         { return GREATER_THAN; }
"<="                        { return LESS_EQUAL; }
">="                        { return GREATER_EQUAL; }
">>"                        { return R_SHIFT; }
"<<"                        { return L_SHIFT; }
"="                         { return ASSIGN; }
"+="                        { return PLUS_ASSIGN; }
"-="                        { return MINUS_ASSIGN; }
";"                         { return SEMICOLON; }
","                         { return COMMA; }
":"                         { return COLON; }
"("                         { return L_PAREN; }
")"                         { return R_PAREN; }
"{"                         { return L_CURLY_BRACKET; }
"}"                         { return R_CURLY_BRACKET; }
"["                         { return L_SQUARE_BRACKET; }
"]"                         { return R_SQUARE_BRACKET; }
"?"                         { return TERNARY_CONDITIONAL; }
"#"                         { return NUMBER_SIGN; }
"->"                        { return POINTER; }

\"(\\.|[^"\\])*\"	        { yylval.string = strdup(yytext); return STRING; }
'([^'\\\n]|\\.)'            { yylval.character = yytext[0]; return CHARACTER; }

[_a-zA-Z][_a-zA-Z0-9]*      { yylval.string = strdup(yytext); return ID; }

[0-9]+                      { yylval.integer = atoi(yytext); return NUM_INTEGER; }

.                           { printf("UNKNOWN\n"); }

%%
