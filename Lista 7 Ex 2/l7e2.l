/* Nao usar a biblioteca do flex*/
%option noyywrap

%{
    char nextline[1] = "";

    #define PRINT_YYTEXT {printf("%s%s", nextline, yytext); nextline[0] = '\n';}

    #define PRINT_WHITESPACE {printf("%swhite space", nextline); nextline[0] = '\n';}
    #define PRINT_ERROR {printf("%s%s error", nextline, yytext); nextline[0] = '\n';}
    
    #define PRINT_TOKEN(token) {printf("%s%s %s", nextline, yytext, token); nextline[0] = '\n';}
%}

%%

" "                         { PRINT_WHITESPACE }
([\n]|[\r])              
"if"                        { PRINT_TOKEN("IF") }
[a-z][a-z0-9]*              { PRINT_TOKEN("ID") }
[0-9]+                      { PRINT_TOKEN("NUM") }
[0-9]+"."[0-9]+             { PRINT_TOKEN("REAL") }
"--"[a-z]*([\n]|[\r])       { yytext[strlen(yytext)-1] = '\0'; PRINT_TOKEN("comment") }
"-"                         { PRINT_ERROR }
.                           { PRINT_ERROR }

%%

int main(int argc, char** argv){

    yylex();

    return 0;
}
