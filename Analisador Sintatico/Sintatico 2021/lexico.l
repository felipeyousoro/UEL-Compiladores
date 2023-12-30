/* Nao usar a biblioteca do flex*/
%option noyywrap
%option yylineno

%{
    #include <sintatico.tab.h>
    #include <string.h>
    extern YYSTYPE yylval; /* YYSTYPE possui tipo int por padrao */

    int yycolno = 1;
    int comment_start_line = 1, comment_start_col = 1;
    int last_token_line, last_token_col;

    #define ADD_COLNO { yycolno += yyleng; }
    #define MOVE_ENDING_POINTER { last_token_line = yylineno; last_token_col = yycolno + 1; }

    char nextline[1] = "";
    #define PRINT_COMMENT_WARNING {printf("%swarning:%i:%i: '/*' within block comment", nextline, yylineno, yycolno); nextline[0] = '\n';}

%}

%x COMMENT_BLOCK
%x COMMENT_LINE

%%

<INITIAL>{

("\n"|"\r")                 { yycolno = 1; }
" "                         { ADD_COLNO; }

void                        { MOVE_ENDING_POINTER; ADD_COLNO; return VOID; } 
int                         { MOVE_ENDING_POINTER; ADD_COLNO; return INT; }
char                        { MOVE_ENDING_POINTER; ADD_COLNO; return CHAR; }
return                      { MOVE_ENDING_POINTER; ADD_COLNO; return RETURN; }
break                       { MOVE_ENDING_POINTER; ADD_COLNO; return BREAK; }  
switch                      { MOVE_ENDING_POINTER; ADD_COLNO; return SWITCH; }
case                        { MOVE_ENDING_POINTER; ADD_COLNO; return CASE; }
default                     { MOVE_ENDING_POINTER; ADD_COLNO; return DEFAULT; }
do                          { MOVE_ENDING_POINTER; ADD_COLNO; return DO; }
while                       { MOVE_ENDING_POINTER; ADD_COLNO; return WHILE; }
for                         { MOVE_ENDING_POINTER; ADD_COLNO; return FOR; }
if                          { MOVE_ENDING_POINTER; ADD_COLNO; return IF; }
else                        { MOVE_ENDING_POINTER; ADD_COLNO; return ELSE; }
typedef                     { MOVE_ENDING_POINTER; ADD_COLNO; return TYPEDEF; }
struct                      { MOVE_ENDING_POINTER; ADD_COLNO; return STRUCT; }
"+"                         { MOVE_ENDING_POINTER; ADD_COLNO; return PLUS; }
"-"                         { MOVE_ENDING_POINTER; ADD_COLNO; return MINUS; }
"*"                         { MOVE_ENDING_POINTER; ADD_COLNO; return MULTIPLY; }
"/"                         { MOVE_ENDING_POINTER; ADD_COLNO; return DIV; }
"%"                         { MOVE_ENDING_POINTER; ADD_COLNO; return REMAINDER; }
"++"                        { MOVE_ENDING_POINTER; ADD_COLNO; return INC; }
"--"                        { MOVE_ENDING_POINTER; ADD_COLNO; return DEC; }
"&"                         { MOVE_ENDING_POINTER; ADD_COLNO; return BITWISE_AND; }
"|"                         { MOVE_ENDING_POINTER; ADD_COLNO; return BITWISE_OR; }
"~"                         { MOVE_ENDING_POINTER; ADD_COLNO; return BITWISE_NOT; }
"^"                         { MOVE_ENDING_POINTER; ADD_COLNO; return BITWISE_XOR; }
"!"                         { MOVE_ENDING_POINTER; ADD_COLNO; return NOT; }  
"&&"                        { MOVE_ENDING_POINTER; ADD_COLNO; return LOGICAL_AND; }
"||"                        { MOVE_ENDING_POINTER; ADD_COLNO; return LOGICAL_OR; }
"=="                        { MOVE_ENDING_POINTER; ADD_COLNO; return EQUAL; }
"!="                        { MOVE_ENDING_POINTER; ADD_COLNO; return NOT_EQUAL; }
"<"                         { MOVE_ENDING_POINTER; ADD_COLNO; return LESS_THAN; }
">"                         { MOVE_ENDING_POINTER; ADD_COLNO; return GREATER_THAN; }
"<="                        { MOVE_ENDING_POINTER; ADD_COLNO; return LESS_EQUAL; }  
">="                        { MOVE_ENDING_POINTER; ADD_COLNO; return GREATER_EQUAL; }
">>"                        { MOVE_ENDING_POINTER; ADD_COLNO; return R_SHIFT; }
"<<"                        { MOVE_ENDING_POINTER; ADD_COLNO; return L_SHIFT; }
"="                         { MOVE_ENDING_POINTER; ADD_COLNO; return ASSIGN; }
"+="                        { MOVE_ENDING_POINTER; ADD_COLNO; return ADD_ASSIGN; }
"-="                        { MOVE_ENDING_POINTER; ADD_COLNO; return MINUS_ASSIGN; }
";"                         { MOVE_ENDING_POINTER; ADD_COLNO; return SEMICOLON; }
","                         { MOVE_ENDING_POINTER; ADD_COLNO; return COMMA; }
":"                         { MOVE_ENDING_POINTER; ADD_COLNO; return COLON; }
"("                         { MOVE_ENDING_POINTER; ADD_COLNO; return L_PAREN; }
")"                         { MOVE_ENDING_POINTER; ADD_COLNO; return R_PAREN; }
"{"                         { MOVE_ENDING_POINTER; ADD_COLNO; return L_CURLY_BRACKET; }
"}"                         { MOVE_ENDING_POINTER; ADD_COLNO; return R_CURLY_BRACKET; }
"["                         { MOVE_ENDING_POINTER; ADD_COLNO; return L_SQUARE_BRACKET; }
"]"                         { MOVE_ENDING_POINTER; ADD_COLNO; return R_SQUARE_BRACKET; }
"?"                         { MOVE_ENDING_POINTER; ADD_COLNO; return TERNARY_CONDITIONAL; }
"#"                         { MOVE_ENDING_POINTER; ADD_COLNO; return NUMBER_SIGN; }
"->"                        { MOVE_ENDING_POINTER; ADD_COLNO; return POINTER; }
printf                      { MOVE_ENDING_POINTER; ADD_COLNO; return PRINTF; }
scanf                       { MOVE_ENDING_POINTER; ADD_COLNO; return SCANF; }
define                      { MOVE_ENDING_POINTER; ADD_COLNO; return DEFINE; }
exit                        { MOVE_ENDING_POINTER; ADD_COLNO; return EXIT; }



\"(\\.|[^"\\])*\"	        { MOVE_ENDING_POINTER; ADD_COLNO; return STRING; }
'([^'\\\n]|\\.)'            { MOVE_ENDING_POINTER; ADD_COLNO; return CHARACTER; }

[_a-zA-Z][_a-zA-Z0-9]*      { if (yyleng > 255) { return LONG_IDENTIFIER; } else MOVE_ENDING_POINTER; ADD_COLNO; return IDENTIFIER; }

"0"[1-7]+                   { MOVE_ENDING_POINTER; ADD_COLNO; return NUM_OCTAL; }
"0"(x|X)[0-9a-fA-F]+        { MOVE_ENDING_POINTER; ADD_COLNO; return NUM_HEXA; }
[0-9]+                      { MOVE_ENDING_POINTER; ADD_COLNO; return NUM_INTEGER; }

"/*"                        { BEGIN COMMENT_BLOCK; ADD_COLNO; comment_start_line = yylineno;\
                              comment_start_col = yycolno - 2;}

"//"                        { BEGIN COMMENT_LINE; ADD_COLNO; comment_start_line == yylineno;\
                              comment_start_col = yycolno - 2;\
                              last_token_line = yylineno; last_token_col = yycolno - 2;}

.                           { return UNKNOWN_CHARACTER; }

<<EOF>>                     { return END_OF_FILE; }

}

<COMMENT_BLOCK>
{

"/"*"*/"                { BEGIN(INITIAL); ADD_COLNO; last_token_line = yylineno;\
                          last_token_col = yycolno; }

"/*"                    { PRINT_COMMENT_WARNING; ADD_COLNO;}

"\n"                    { yycolno = 1; }

<<EOF>>                 { return UNTERMINATED_COMMENT; }

.                       { ADD_COLNO; }

}


<COMMENT_LINE>
{

"\n"                    { BEGIN(INITIAL); yycolno = 1; }

.                       { ADD_COLNO; }

<<EOF>>                 { BEGIN(INITIAL); return END_OF_FILE; }

}

%%
