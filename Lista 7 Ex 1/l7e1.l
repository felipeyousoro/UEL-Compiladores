/* Nao usar a biblioteca do flex*/
%option noyywrap

%{
    char nextline[1] = "";

    #define PRINT_YYTEXT {printf("%s%s", nextline, yytext); nextline[0] = '\n';}
    #define PRINT_ERROR {printf("%sERRO", nextline); nextline[0] = '\n';}
%}

%%

"+"                         { PRINT_YYTEXT }
"-"                         { PRINT_YYTEXT }
"\n"
"\r"  
[a-z][a-z0-9]*              { PRINT_YYTEXT }
[0-9]+                      { PRINT_YYTEXT }
[0-9]+"e"("+"|"-")?[0-9]+   { PRINT_YYTEXT }
.                           { PRINT_ERROR }

%%

int main(int argc, char** argv){

    yylex();

    return 0;
}
