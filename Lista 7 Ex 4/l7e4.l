/* Nao usar a biblioteca do flex*/
%option noyywrap yylineno

%{
    int semicolons= 0;
    int includes_defines = 0;
    int blank_lines = 0;
    int comment_lines = 0;
    int nb_nc_lines = 0;

    int total_semicolons = 0;
    int total_includes_defines = 0;
    int total_blank_lines = 0;
    int total_comment_lines = 0;
    int total_nb_nc_lines = 0;
    int total_lines = 0;
%}

%x COMMENT
%x POST_COMMENT
%x LINE_COMMENT

%%

<INITIAL>{
    ("#include"|"#define") {includes_defines++;}
    ";"                    {semicolons++;}
    ^[ ]*"\n"              {blank_lines++;}
    "\n"                   {nb_nc_lines++;}

    "/*"                   {BEGIN(COMMENT);}
    "//"                   {BEGIN(LINE_COMMENT);}

    <<EOF>>                {nb_nc_lines++; return 0;}

    .                      {}

}

<COMMENT>{
    ("#include"|"#define") {includes_defines++;}
    ";"                    {semicolons++;}
    ^[ ]*"\n"              {comment_lines++; blank_lines++;}
    "\n"                   {comment_lines++;}

    "/"*"*/"               {BEGIN(POST_COMMENT);}

    .                      {}

}

<POST_COMMENT>{
    ("#include"|"#define") {includes_defines++;}
    ";"                    {semicolons++;}
    "\n"                   {comment_lines++; BEGIN(INITIAL);}

    "/*"                   {BEGIN(COMMENT);}
    "//"                   {BEGIN(LINE_COMMENT);}

    <<EOF>>                {comment_lines++; return 0;}

    .                      {}

}

<LINE_COMMENT>{
    ("#include"|"#define") {includes_defines++;}
    ";"                    {semicolons++;}
    "\n"                   {comment_lines++; BEGIN(INITIAL);}

    <<EOF>>                {comment_lines++; return 0;}

    .                      {}

}



%%

void reset_variables(){
    total_semicolons += semicolons;
    total_includes_defines += includes_defines;
    total_blank_lines += blank_lines;
    total_comment_lines += comment_lines;
    total_nb_nc_lines += nb_nc_lines;
    total_lines += yylineno;


    yylineno = 1;
    semicolons = 0;
    includes_defines = 0;
    blank_lines = 0;
    comment_lines = 0;
    nb_nc_lines = 0;
}


int main(int argc, char** argv){
    /*  */ /*  */
    
    printf(" total    blank lines w/   nb, nc    semi- preproc. file\n");
	printf(" lines    lines comments    lines   colons  direct.\n");
	printf("------+--------+--------+--------+--------+--------+----\n");

    for(int i = 1; i < argc; i++){
        yyin =  fopen(argv[i], "r");

        yylex();

        fclose(yyin);

        printf("%6i %8i %8i %8i %8i %8i %s\n", yylineno, blank_lines, comment_lines, nb_nc_lines, semicolons, includes_defines, argv[i]);

        reset_variables();

    }

    printf("%6i %8i %8i %8i %8i %8i %s\n", total_lines, total_blank_lines, total_comment_lines, total_nb_nc_lines, total_semicolons, total_includes_defines, "total");
    




    return 0;
}
