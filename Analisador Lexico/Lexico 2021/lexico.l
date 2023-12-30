/* Nao usar a biblioteca do flex*/
%option noyywrap
%option yylineno

%{
    char nextline[1] = "";
    int yycolno = 1, comment_block_start_line = 0, comment_block_start_col = 0;

    char* remove_first_and_last_char(char* str) {
        char aux[strlen(str)];

        for (int i = 1; i < strlen(str) - 1; i++) {
            aux[i - 1] = str[i];
        }

        aux[strlen(str) - 2] = '\0';

        strcpy(str, aux);

        return str;
    }

    #define AFTER_ACTION {nextline[0] = '\n'; yycolno += yyleng;}

    #define PRINT_TOKEN(token) {printf("%s%s", nextline, token); AFTER_ACTION}
    #define PRINT_TOKEN_WITH_CONTENT(token) {printf("%s%s(%s)", nextline, token, yytext); AFTER_ACTION}

    #define PRINT_TOKEN_BETWEEN(token) {printf("%s%s(%s)", nextline, token, remove_first_and_last_char(yytext)); AFTER_ACTION}

    #define PRINT_UNKNOWN_CHARACTER {printf("%serror:lexical:%i:%i: %s", nextline, yylineno, yycolno, yytext); AFTER_ACTION}

    #define PRINT_IDENTIFIER_TOO_LONG {printf("%serror:lexical:%i:%i: identifier too long", nextline, yylineno, yycolno); AFTER_ACTION}
    
    #define PRINT_COMMENT_BLOCK_ERROR {printf("%serror:lexical:%i:%i: unterminated comment", nextline, comment_block_start_line, comment_block_start_col);}
    #define PRINT_COMMENT_WARNING {printf("%swarning:%i:%i: '/*' within block comment", nextline, yylineno, yycolno);}

%}

%x COMMENT_BLOCK
%x COMMENT_LINE

%%

<INITIAL>{

("\n"|"\r")                 {yycolno = 1;}
" "                         {yycolno += 1;}

void                        {PRINT_TOKEN("VOID")}
int                         {PRINT_TOKEN("INT")}
char                        {PRINT_TOKEN("CHAR")}
return                      {PRINT_TOKEN("RETURN")}
break                       {PRINT_TOKEN("BREAK")}
switch                      {PRINT_TOKEN("SWITCH")}
case                        {PRINT_TOKEN("CASE")}
default                     {PRINT_TOKEN("DEFAULT")}
do                          {PRINT_TOKEN("DO")}
while                       {PRINT_TOKEN("WHILE")}
for                         {PRINT_TOKEN("FOR")}
if                          {PRINT_TOKEN("IF")}
else                        {PRINT_TOKEN("ELSE")}
typedef                     {PRINT_TOKEN("TYPEDEF")}
struct                      {PRINT_TOKEN("STRUCT")}
"+"                         {PRINT_TOKEN("PLUS")}
"-"                         {PRINT_TOKEN("MINUS")}
"*"                         {PRINT_TOKEN("MULTIPLY")}
"/"                         {PRINT_TOKEN("DIV")}
"%"                         {PRINT_TOKEN("REMAINDER")}
"++"                        {PRINT_TOKEN("INC")}
"--"                        {PRINT_TOKEN("DEC")}
"&"                         {PRINT_TOKEN("BITWISE_AND")}
"|"                         {PRINT_TOKEN("BITWISE_OR")}
"~"                         {PRINT_TOKEN("BITWISE_NOT")}
"^"                         {PRINT_TOKEN("BITWISE_XOR")}
"!"                         {PRINT_TOKEN("NOT")}
"&&"                        {PRINT_TOKEN("LOGICAL_AND")}
"||"                        {PRINT_TOKEN("LOGICAL_OR")}
"=="                        {PRINT_TOKEN("EQUAL")}
"!="                        {PRINT_TOKEN("NOT_EQUAL")}
"<"                         {PRINT_TOKEN("LESS_THAN")}
">"                         {PRINT_TOKEN("GREATER_THAN")}
"<="                        {PRINT_TOKEN("LESS_EQUAL")}
">="                        {PRINT_TOKEN("GREATER_EQUAL")}
">>"                        {PRINT_TOKEN("R_SHIFT")}
"<<"                        {PRINT_TOKEN("L_SHIFT")}
"="                         {PRINT_TOKEN("ASSIGN")}
"+="                        {PRINT_TOKEN("ADD_ASSIGN")}
"-="                        {PRINT_TOKEN("MINUS_ASSIGN")}
";"                         {PRINT_TOKEN("SEMICOLON")}
","                         {PRINT_TOKEN("COMMA")}
":"                         {PRINT_TOKEN("COLON")}
"("                         {PRINT_TOKEN("L_PAREN")}
")"                         {PRINT_TOKEN("R_PAREN")}
"{"                         {PRINT_TOKEN("L_CURLY_BRACKET")}
"}"                         {PRINT_TOKEN("R_CURLY_BRACKET")}
"["                         {PRINT_TOKEN("L_SQUARE_BRACKET")}
"]"                         {PRINT_TOKEN("R_SQUARE_BRACKET")}
"?"                         {PRINT_TOKEN("TERNARY_CONDITIONAL")}
"#"                         {PRINT_TOKEN("NUMBER_SIGN")}
"->"                        {PRINT_TOKEN("POINTER")}
printf                      {PRINT_TOKEN("PRINTF")}
scanf                       {PRINT_TOKEN("SCANF")}
define                      {PRINT_TOKEN("DEFINE")}
exit                        {PRINT_TOKEN("EXIT")}



\"(\\.|[^"\\])*\"	        {PRINT_TOKEN_BETWEEN("STRING")}
'([^'\\\n]|\\.)'            {PRINT_TOKEN_BETWEEN("CHARACTER")}

[_a-zA-Z][_a-zA-Z0-9]*      {if (yyleng > 255) {PRINT_IDENTIFIER_TOO_LONG} else {PRINT_TOKEN_WITH_CONTENT("IDENTIFIER")}}

"0"[1-7]+                   {PRINT_TOKEN_WITH_CONTENT("NUM_OCTAL")}
"0"(x|X)[0-9a-fA-F]+        {PRINT_TOKEN_WITH_CONTENT("NUM_HEXA")}
[0-9]+                      {PRINT_TOKEN_WITH_CONTENT("NUM_INTEGER")}

"/*"                        {BEGIN COMMENT_BLOCK; yycolno +=2; comment_block_start_line = yylineno;\
                             comment_block_start_col = yycolno - 2;}

"//"                        {BEGIN COMMENT_LINE; yycolno +=2;}


.                           {PRINT_UNKNOWN_CHARACTER}

}

<COMMENT_BLOCK>
{
    
"/"*"*/"                {BEGIN(INITIAL); yycolno += yyleng;}
"/*"                    {PRINT_COMMENT_WARNING; yycolno +=2;}

"\n"                    {yycolno = 1;}

<<EOF>>                 {PRINT_COMMENT_BLOCK_ERROR; return 0;}

.                       {yycolno++;}

}

<COMMENT_LINE>
{

"\n"                    {yycolno = 1; BEGIN(INITIAL);}

.                       {yycolno++;}

}


%%

int main(int argc, char** argv){

    yylex();

    return 0;
}
