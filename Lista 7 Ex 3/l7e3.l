/* Nao usar a biblioteca do flex*/
%option noyywrap

%{
    char newline[1] = "";
%}

%%

(aba)+                                      {printf("%saction 1", newline); newline[0] = '\n';}
(a(b)*a)                                    {printf("%saction 2", newline); newline[0] = '\n';}
(a|b)                                       {printf("%saction 3", newline); newline[0] = '\n';}

%%

int main(int argc, char** argv){

    yylex();

    return 0;
}
