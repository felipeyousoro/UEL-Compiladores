#include <iostream>
#include <vector>
#include "matriz.cpp"

using namespace std;

#define ERRO_SINTATICO this->col_no = this->col_no - this->token_content.length(); cout << "ERRO DE SINTAXE. Linha: " << this->line_no << " Coluna: " << this->col_no << " -> \'" << this->token_content << "\'"; exit(0);
#define ERRO_LEXICO this->col_no = this->col_no - this->token_content.length(); cout << "ERRO LEXICO. Linha: " << this->line_no << " Coluna: " << this->col_no << " -> \'" << this->token_content << "\'"; exit(0);

class Automaton{
    private:
        int initial_state, current_index, token;
        //int **transition_table;      
        int line_no, col_no;
        string input, token_content;

        int get_next_token(){
            int last_final_state = 0, last_final_index = this->current_index, current_state = this->initial_state;

            for(int i = this->current_index; i < this->input.length(); i++){
                int next_state = automaton[current_state][input[i]];
                if(final_states[next_state] == true){
                    last_final_state = next_state;
                    last_final_index = i;
                }

                if(tokens[next_state] == ERRO){
                    this->token_content = this->input.substr(this->current_index, i - this->current_index);
                    this->current_index = last_final_index + 1;
                    return tokens[last_final_state];
                }

                current_state = next_state;
            }

            this->token_content = this->input.substr(this->current_index, this->input.length() - this->current_index);
            this->current_index = last_final_index + 1;
            return tokens[last_final_state];
        }

        void count_comment_length(){
            //iterate over comment counting lines and columns
            // cout << "start count_comment_length" << endl;
            //print col no
            //cout << this->col_no << endl;
            for(int i = 0; i < this->token_content.length(); i++){
                if(this->token_content[i] == '\n'){
                    this->line_no++;
                    // cout << "oi" << endl;
                    this->col_no = 1;
                }
                else{
                    this->col_no++;
                }
            }
            // cout << "end count_comment_length" << endl;
        }

        void advance(){
            this->token = get_next_token();

            if(this->token == EOL){
                // cout << "fim de linha" << endl;
                this->line_no++;
                this->col_no = 1;
                }
            else if(this->token == COMMENT){
                count_comment_length();
            }
            else{
                // cout << "conteudo: " << this->token_content << endl;
                this->col_no += this->token_content.length();
            }

            while(this->token == COMMENT || this->token == WHITESPACE || this->token == EOL || this->token == OTHER){
                if(this->token == OTHER){
                    ERRO_LEXICO;
                }

                this->token = get_next_token();
                if(this->token == EOL){
                    this->line_no++;
                    this->col_no = 1;
                }
                else if(this->token == COMMENT){
                    count_comment_length();
                }
                else{
                    this->col_no += this->token_content.length();
                }
                
            }

        }

        void eat(int token){
            //cout << "eat: " << this->token << endl;
            if(this->token == token){
                advance();
            }
            else{
                ERRO_SINTATICO;

            }
        }

        void programa(){
            eat(ALGORITMO); eat(ID); eat(SEMICOLON); bloco_variaveis(); \
                    procedimento_funcao(); bloco_comandos(); eat(DOT); cout << "PROGRAMA CORRETO.";
            
        }

        void procedimento_funcao(){
            // cout << "estou em procedimento_funcao" << endl;
            switch(this->token){
                case(PROCEDIMENTO): declara_procedimento(); procedimento_funcao(); break;
                case(FUNCAO): declara_funcao(); procedimento_funcao(); break;
                case(INICIO): break;

                default: ERRO_SINTATICO; // cout << "erro em procedimento_funcao" << endl; exit(1);
            }

        }

        void declara_procedimento(){
            // cout << "estou em declara_procedimento" << endl;
            switch(this->token){
                case(PROCEDIMENTO): eat(PROCEDIMENTO); eat(ID); parametros(); eat(SEMICOLON); declara_parametros(); bloco_variaveis(); bloco_comandos(); eat(SEMICOLON); break;

                default: ERRO_SINTATICO; // cout << "erro em declara_procedimento" << endl; exit(1);
            }

        }

        void declara_funcao(){
            // cout << "estou em declara_funcao" << endl;
            switch(this->token){
                case(FUNCAO): eat(FUNCAO); eat(ID); parametros(); eat(COLON); tipo_basico(); eat(SEMICOLON); declara_parametros(); bloco_variaveis(); bloco_comandos(); eat(SEMICOLON); break;
                
                default: ERRO_SINTATICO; // cout << "erro em declara_funcao" << endl; exit(1);
            }
        }

        void parametros(){
            // cout << "estou em parametros" << endl;
            switch(this->token){
                case(L_C_BRACKET): eat(L_C_BRACKET); declara_identificador(); eat(R_C_BRACKET); break;
                case(SEMICOLON): break;
                case(COLON): break;

                default: ERRO_SINTATICO; // cout << "erro em parametros" << endl; exit(1);
            }

        }

        void declara_parametros(){
            // cout << "estou em declara_parametros" << endl;
            switch(this->token){
                case(ID): declaracoes(); break;
                case(TIPO): declaracoes(); break;
                case(INTEIRO): declaracoes(); break;
                case(REAL): declaracoes(); break;
                case(CARACTERE): declaracoes(); break;
                case(LOGICO): declaracoes(); break;
                case(PROCEDIMENTO): break; 
                case(FUNCAO): break;
                case(VARIAVEIS): break;
                case(INICIO): break;

                default: ERRO_SINTATICO; // cout << "erro em declara_parametros" << endl; exit(1);
            }

        }

        void bloco_variaveis(){
            // cout << "estou em bloco_variaveis" << endl;
            switch(this->token){
                case(VARIAVEIS): eat(VARIAVEIS); declaracoes(); break;
                case(PROCEDIMENTO): break;
                case(FUNCAO): break;
                case(INICIO): break;
                case(_EOF): break;

                default: ERRO_SINTATICO; // cout << "erro em bloco_variaveis" << endl; exit(1);
            }
        }

        void declaracoes(){
            // cout << "estou em declaracoes" << endl;
            // cout << "token: " << this->token << endl;
            // cout << "conteudo: " << this->token_content << endl;
            switch(this->token){
                case(ID): declara_variaveis(); declaracoes_aux(); break;
                case(INTEIRO): declara_variaveis(); declaracoes_aux(); break;
                case(REAL): declara_variaveis(); declaracoes_aux(); break;
                case(CARACTERE): declara_variaveis(); declaracoes_aux(); break;
                case(LOGICO): declara_variaveis(); declaracoes_aux(); break;
                case(TIPO): declara_tipo(); declaracoes_aux(); break;

                default: ERRO_SINTATICO; // cout << "erro em declaracoes" << endl; exit(1);
            }

        }

        void declaracoes_aux(){
            // cout << "estou em declaracoes_aux" << endl;
            switch(this->token){
                case(ID): declara_variaveis(); declaracoes_aux(); break;
                case(INTEIRO): declara_variaveis(); declaracoes_aux(); break;
                case(REAL): declara_variaveis(); declaracoes_aux(); break;
                case(CARACTERE): declara_variaveis(); declaracoes_aux(); break;
                case(LOGICO): declara_variaveis(); declaracoes_aux(); break;
                case(TIPO): declara_tipo(); declaracoes_aux(); break;
                case(PROCEDIMENTO): break;
                case(FUNCAO): break;
                case(VARIAVEIS): break;
                case(INICIO): break;
                case(_EOF): break;

                default: ERRO_SINTATICO; // cout << "erro em declaracoes_aux" << endl; exit(1);
            }

        }

        void declara_tipo(){
            // cout << "estou em declara_tipo" << endl;
            switch(this->token){
                case(TIPO): eat(TIPO); eat(ID); eat(EQUAL); vetor_matriz(); eat(L_S_BRACKET); dimensao(); eat(R_S_BRACKET); tipo_basico(); eat(SEMICOLON); break;

                default: ERRO_SINTATICO; // cout << "erro em declara_tipo" << endl; exit(1);
            }
        }

        void declara_variaveis(){
            // cout << "estou em declara_variaveis" << endl;
            switch(this->token){
                case(ID): tipo_basico(); eat(COLON); declara_identificador(); eat(SEMICOLON); break; 
                case(INTEIRO): tipo_basico(); eat(COLON); declara_identificador(); eat(SEMICOLON); break; 
                case(REAL): tipo_basico(); eat(COLON); declara_identificador(); eat(SEMICOLON); break; 
                case(CARACTERE): tipo_basico(); eat(COLON); declara_identificador(); eat(SEMICOLON); break; 
                case(LOGICO): tipo_basico(); eat(COLON); declara_identificador(); eat(SEMICOLON); break; 

                default: ERRO_SINTATICO; // cout << "erro em declara_variaveis" << endl; exit(1);
            }

            // cout << "saiu de declara_variaveis" << endl;
        }

        void declara_identificador(){
            // cout << "estou em declara_identificador" << endl;
            // cout << "token: " << this->token << endl;
            // cout << "conteudo: " << this->token_content << endl;
            switch(this->token){
                case(ID): eat(ID); declara_identificador_aux(); break;

                default: ERRO_SINTATICO; // cout << "erro em declara_identificador" << endl; exit(1);
            }
        }

        void declara_identificador_aux(){
            // cout << "estou em declara_identificador_aux" << endl;
            // cout << "token: " << this->token << endl;
            //cout << "conteudo: " << this->token_content << endl;
            switch(this->token){
                case(COMMA): eat(COMMA); declara_identificador(); break;
                case(SEMICOLON): break;
                case(R_C_BRACKET): break;

                default: ERRO_SINTATICO; // cout << "erro em declara_identificador_aux" << endl; exit(1);
            }
        }

        void vetor_matriz(){
            // cout << "estou em vetor_matriz" << endl;
            // cout << "token: " << this->token << endl;
            switch(this->token){
                case(VETOR): eat(VETOR); break;
                case(MATRIZ): eat(MATRIZ); break;

                default: ERRO_SINTATICO; // cout << "erro em vetor_matriz" << endl; exit(1);
            }
        }

        void dimensao(){
            // cout << "estou em dimensao" << endl;
            switch(this->token){
                case(NUM_INT): eat(NUM_INT); eat(COLON); eat(NUM_INT); dimensao_aux(); break;

                default: ERRO_SINTATICO; // cout << "erro em dimensao" << endl; exit(1);
            }
        }

        void dimensao_aux(){
            // cout << "estou em dimensao_aux" << endl;
            switch(this->token){
                case(COMMA): eat(COMMA); dimensao(); break;
                case(R_S_BRACKET): break;

                default: ERRO_SINTATICO; // cout << "erro em dimensao_aux" << endl; exit(1);
            }   
        }

        void tipo_basico(){
            // cout << "estou em tipo_basico" << endl;
            // cout << "token: " << this->token << endl;
            // cout << "conteudo: " << this->token_content << endl;
            switch(this->token){
                case(ID): eat(ID); break;
                case(INTEIRO): eat(INTEIRO); break;
                case(REAL): eat(REAL); break;
                case(CARACTERE): eat(CARACTERE); break;
                case(LOGICO): eat(LOGICO); break;

                default: ERRO_SINTATICO; // cout << "erro em tipo_basico" << endl; exit(1);
            }
        }

        void bloco_comandos(){
            switch(this->token){
                case(INICIO): eat(INICIO); lista_comandos(); eat(FIM); break;

                default: ERRO_SINTATICO; // cout << "erro em bloco_comandos" << endl; exit(1);
            }
        }

        void lista_comandos(){
            // cout << "estou em lista_comandos" << endl;
            switch(this->token){
                case(ID): comandos(); eat(SEMICOLON); lista_comandos_aux(); break;
                case(SE): comandos(); eat(SEMICOLON); lista_comandos_aux(); break;
                case(ENQUANTO): comandos(); eat(SEMICOLON); lista_comandos_aux(); break;
                case(REPITA): comandos(); eat(SEMICOLON); lista_comandos_aux(); break;
                case(PARA): comandos(); eat(SEMICOLON); lista_comandos_aux(); break;
                case(LEIA): comandos(); eat(SEMICOLON); lista_comandos_aux(); break;
                case(IMPRIMA): comandos(); eat(SEMICOLON); lista_comandos_aux(); break;

                default: ERRO_SINTATICO; // cout << "erro em lista_comandos" << endl; exit(1);
            }
            
        }

        void lista_comandos_aux(){
            // cout << "estou em lista_comandos_aux" << endl;
            switch(this->token){
                case(ID): comandos(); eat(SEMICOLON); lista_comandos_aux(); break;
                case(SE): comandos(); eat(SEMICOLON); lista_comandos_aux(); break;
                case(ENQUANTO): comandos(); eat(SEMICOLON); lista_comandos_aux(); break;
                case(REPITA): comandos(); eat(SEMICOLON); lista_comandos_aux(); break;
                case(PARA): comandos(); eat(SEMICOLON); lista_comandos_aux(); break;
                case(LEIA): comandos(); eat(SEMICOLON); lista_comandos_aux(); break;
                case(IMPRIMA): comandos(); eat(SEMICOLON); lista_comandos_aux(); break;
                case(FIM): break;
                case(ATE): break;
                case(SENAO): break;

                default: ERRO_SINTATICO; // cout << "erro em lista_comandos_aux" << endl; exit(1);
            }
        }

        void comandos(){
            // cout << "estou em comandos" << endl;
            switch(this->token){
                case(ID): eat(ID); comandos_aux_id(); break;
                case(SE): eat(SE); expressao(); eat(ENTAO); lista_comandos(); comandos_aux_se(); break;
                case(ENQUANTO): eat(ENQUANTO); expressao(); eat(FACA); lista_comandos(); eat(FIM); eat(ENQUANTO); break;
                case(PARA): eat(PARA); eat(ID); eat(DE); expressao(); eat(ATE); expressao(); comandos_aux_para(); break;
                case(REPITA): eat(REPITA); lista_comandos(); eat(ATE); expressao(); break;
                case(LEIA): eat(LEIA); eat(L_C_BRACKET); variavel(); eat(R_C_BRACKET); break;
                case(IMPRIMA): eat(IMPRIMA); eat(L_C_BRACKET); expr_iter(); eat(R_C_BRACKET); break;

                default: ERRO_SINTATICO; // cout << "erro em comandos" << endl; exit(1);
            }
        }

        void comandos_aux_id(){
            // cout << "estou em comandos_aux_id" << endl;
            // cout << "token: " << this->token << endl;
            // cout << "conteudo: " << this->token_content << endl;
            switch(this->token){
                case(SEMICOLON): break;
                case(ASSIGN): eat(ASSIGN); expressao(); break;
                case(L_C_BRACKET): eat(L_C_BRACKET); expr_iter(); eat(R_C_BRACKET); break;
                case(L_S_BRACKET): eat(L_S_BRACKET); expr_iter(); eat(R_S_BRACKET); eat(ASSIGN); expressao(); break;

                default: ERRO_SINTATICO; // cout << "erro em comandos_aux_id" << endl; exit(1);
            }
        }

        void comandos_aux_se(){
            // cout << "estou em comandos_aux_se" << endl;
            switch(this->token){
                case(SENAO): eat(SENAO); lista_comandos(); eat(FIM); eat(SE); break;
                case(FIM): eat(FIM); eat(SE); break;

                default: ERRO_SINTATICO; // cout << "erro em comandos_aux_se" << endl; exit(1);
            }
        }

        void comandos_aux_para(){
            // cout << "estou em comandos_aux_para" << endl;
            switch(this->token){
                case(FACA): eat(FACA); lista_comandos(); eat(FIM); eat(PARA); break;
                case(PASSO): eat(PASSO); expressao(); eat(FACA); lista_comandos(); eat(FIM); eat(PARA); break;

                default: ERRO_SINTATICO; // cout << "erro em comandos_aux_para" << endl; exit(1);
            }
        }

        void expressao(){
            // cout << "estou em expressao" << endl;
            switch(this->token){
                case(ID): expressao_simples(); expressao_(); break;
                case(NUM_INT): expressao_simples(); expressao_(); break;
                case(NUM_REAL): expressao_simples(); expressao_(); break;
                case(STRING): expressao_simples(); expressao_(); break;
                case(PLUS): expressao_simples(); expressao_(); break;
                case(MINUS): expressao_simples(); expressao_(); break;
                case(NAO): expressao_simples(); expressao_(); break;
                case(VERDADEIRO): expressao_simples(); expressao_(); break;
                case(FALSO): expressao_simples(); expressao_(); break;
                case(L_C_BRACKET): expressao_simples(); expressao_(); break;

                default: ERRO_SINTATICO; // cout << "erro em expressao" << endl; exit(1);
            }
        }

        void expressao_simples(){
            // cout << "estou em expressao_simples" << endl;
            // cout << "token: " << this->token << endl;
            // cout << "conteudo: " << this->token_content << endl;
            switch(this->token){
                case(ID): termo(); expressao_simples_(); break;
                case(NUM_INT): termo(); expressao_simples_(); break;
                case(NUM_REAL): termo(); expressao_simples_(); break;
                case(STRING): termo(); expressao_simples_(); break;
                case(PLUS): eat(PLUS); termo(); expressao_simples_(); break;
                case(MINUS): eat(MINUS); termo(); expressao_simples_(); break;
                case(NAO): termo(); expressao_simples_(); break;
                case(VERDADEIRO): termo(); expressao_simples_(); break;
                case(FALSO): termo(); expressao_simples_(); break;
                case(L_C_BRACKET): termo(); expressao_simples_(); break;
                case(EQUAL): termo(); expressao_simples_(); break;
                
                default: ERRO_SINTATICO; // cout << "erro em expressao_simples" << endl; exit(1);
            }
        }

        void termo(){
            // cout << "estou em termo" << endl;
            switch(this->token){
                case(ID): fator(); termo_(); break;
                case(NUM_INT): fator(); termo_(); break;
                case(NUM_REAL): fator(); termo_(); break;
                case(STRING): fator(); termo_(); break;
                case(NAO): fator(); termo_(); break;
                case(VERDADEIRO): fator(); termo_(); break;
                case(FALSO): fator(); termo_(); break;
                case(L_C_BRACKET): fator(); termo_(); break;

                default: ERRO_SINTATICO; // cout << "erro em termo" << endl; exit(1);
            }
        }

        void fator(){
            // cout << "estou em fator" << endl;
            switch(this->token){
                case(ID): eat(ID); fator_id_aux(); break;
                case(NUM_INT): eat(NUM_INT); break;
                case(NUM_REAL): eat(NUM_REAL); break;
                case(STRING): eat(STRING); break;
                case(NAO): eat(NAO); fator(); break;
                case(VERDADEIRO): eat(VERDADEIRO); break;
                case(FALSO): eat(FALSO); break;
                case(L_C_BRACKET): eat(L_C_BRACKET); expressao(); eat(R_C_BRACKET); break;

                default: ERRO_SINTATICO; // ERRO_SINTATICO;
            }
        }

        void fator_id_aux(){
            // cout << "estou em fator_id_aux" << endl;
            // cout << "token: " << this->token << endl;
            // cout << "conteudo: " << this->token_content << endl;
            switch(this->token){
                case(SEMICOLON): break;
                case(L_C_BRACKET): eat(L_C_BRACKET); expr_iter(); eat(R_C_BRACKET); break;
                case(R_C_BRACKET): break;
                case(L_S_BRACKET): eat(L_S_BRACKET); expr_iter(); eat(R_S_BRACKET); break;
                case(R_S_BRACKET): break;
                case(EQUAL): break;
                case(COMMA): break;
                case(ENTAO): break;
                case(FACA): break;
                case(ATE): break;
                case(PASSO): break;
                case(PLUS): break;
                case(MINUS): break;
                case(DIFFERENT): break;
                case(LESS): break;
                case(LEQ): break;
                case(GREATER): break;
                case(GEQ): break;
                case(OU): break;
                case(MULT): break;
                case(DIVISION): break;
                case(DIV): break;
                case(E): break;

                default: ERRO_SINTATICO; // break;
            }
        }

        void variavel(){
            // cout << "estou em variavel" << endl;
            switch(this->token){
                case(ID): eat(ID); variavel_aux(); break;

                default: ERRO_SINTATICO; // cout << "erro em variavel" << endl; exit(1);
            }
        }

        void variavel_aux(){
            // cout << "estou em variavel_aux" << endl;
            switch(this->token){
                case(R_C_BRACKET): break;
                case(L_S_BRACKET): eat(L_S_BRACKET); expr_iter(); eat(R_S_BRACKET); break;

                default: ERRO_SINTATICO; // cout << "erro em variavel_aux" << endl; exit(1);
            }
            
        }

        void expr_iter(){
            // cout << "estou em expr_iter" << endl;
            // cout << "token: " << this->token << endl;
            // cout << "conteudo: " << this->token_content << endl;
            switch(this->token){
                case(ID): eat(ID); expr_iter_id_aux(); break;
                case(NUM_INT): eat(NUM_INT); termo_(); expressao_simples_(); expressao_(); expr_iter_comma_expr_loop(); break;
                case(NUM_REAL): eat(NUM_REAL); termo_(); expressao_simples_(); expressao_(); expr_iter_comma_expr_loop(); break;
                case(STRING): eat(STRING); termo_(); expressao_simples_(); expressao_(); expr_iter_comma_expr_loop(); break;
                case(VERDADEIRO): eat(VERDADEIRO); termo_(); expressao_simples_(); expressao_(); expr_iter_comma_expr_loop(); break;
                case(FALSO): eat(FALSO); termo_(); expressao_simples_(); expressao_(); expr_iter_comma_expr_loop(); break;
                case(PLUS): eat(PLUS); termo_(); expressao_simples_(); expressao_(); expr_iter_comma_expr_loop(); break;
                case(MINUS): eat(MINUS); termo_(); expressao_simples_(); expressao_(); expr_iter_comma_expr_loop(); break;
                case(NAO): eat(NAO); fator(); termo_(); expressao_simples_(); expressao_(); expr_iter_comma_expr_loop(); break;
                case(L_C_BRACKET): eat(L_C_BRACKET); expressao(); eat(R_C_BRACKET); termo_(); expressao_simples_(); expr_iter_comma_expr_loop(); break;

                default: ERRO_SINTATICO; // cout << "erro em expr_iter" << endl; exit(1);
            }   
        }

        void expr_iter_comma_expr_loop(){
            // cout << "estou em expr_iter_comma_expr_loop" << endl;
            switch(this->token){
                case(COMMA): eat(COMMA); expr_iter(); break;
                case(R_C_BRACKET): break;
                case(R_S_BRACKET): break;

                default: ERRO_SINTATICO; // cout << "erro em expr_iter_comma_expr_loop" << endl; exit(1);
            }
        }

        void expr_iter_id_aux(){
            // cout << "estou em expr_iter_id_aux" << endl;
            switch(this->token){
                case(L_C_BRACKET): eat(L_C_BRACKET); expr_iter(); eat(R_C_BRACKET); termo_(); expressao_simples_(); expressao_(); expr_iter_comma_expr_loop(); break;
                case(R_C_BRACKET): termo_(); expressao_simples_(); expressao_(); expr_iter_comma_expr_loop(); break;
                case(L_S_BRACKET): eat(L_S_BRACKET); expr_iter(); eat(R_S_BRACKET); termo_(); expressao_simples_(); expressao_(); expr_iter_comma_expr_loop(); break;
                case(R_S_BRACKET): termo_(); expressao_simples_(); expressao_(); expr_iter_comma_expr_loop(); break;
                case(EQUAL): termo_(); expressao_simples_(); expressao_(); expr_iter_comma_expr_loop(); break;
                case(COMMA): termo_(); expressao_simples_(); expressao_(); expr_iter_comma_expr_loop(); break;
                case(PLUS): termo_(); expressao_simples_(); expressao_(); expr_iter_comma_expr_loop(); break;
                case(MINUS): termo_(); expressao_simples_(); expressao_(); expr_iter_comma_expr_loop(); break;
                case(DIFFERENT): termo_(); expressao_simples_(); expressao_(); expr_iter_comma_expr_loop(); break;
                case(LESS): termo_(); expressao_simples_(); expressao_(); expr_iter_comma_expr_loop(); break;
                case(LEQ): termo_(); expressao_simples_(); expressao_(); expr_iter_comma_expr_loop(); break;
                case(GREATER): termo_(); expressao_simples_(); expressao_(); expr_iter_comma_expr_loop(); break;
                case(GEQ): termo_(); expressao_simples_(); expressao_(); expr_iter_comma_expr_loop(); break;
                case(OU): termo_(); expressao_simples_(); expressao_(); expr_iter_comma_expr_loop(); break;
                case(MULT): termo_(); expressao_simples_(); expressao_(); expr_iter_comma_expr_loop(); break;
                case(DIVISION): termo_(); expressao_simples_(); expressao_(); expr_iter_comma_expr_loop(); break;
                case(DIV): termo_(); expressao_simples_(); expressao_(); expr_iter_comma_expr_loop(); break;
                case(E): termo_(); expressao_simples_(); expressao_(); expr_iter_comma_expr_loop(); break;

                default: ERRO_SINTATICO; // cout << "erro em expr_iter_id_aux" << endl; exit(1);
            }
        }

        void expressao_(){
            // cout << "estou em expressao_" << endl;
            switch(this->token){
                case(R_C_BRACKET): break;
                case(R_S_BRACKET): break;
                case(SEMICOLON): break;
                case(COMMA): break;
                case(ENTAO): break;
                case(FACA): break;
                case(ATE): break;
                case(PASSO): break;
                case(EQUAL): eat(EQUAL); expressao_simples(); expressao_(); break;
                case(DIFFERENT): eat(DIFFERENT); expressao_simples(); expressao_(); break;
                case(LESS): eat(LESS); expressao_simples(); expressao_(); break;
                case(LEQ): eat(LEQ); expressao_simples(); expressao_(); break;
                case(GREATER): eat(GREATER); expressao_simples(); expressao_(); break;
                case(GEQ): eat(GEQ); expressao_simples(); expressao_(); break;

                default: ERRO_SINTATICO; // cout << "erro em expressao_" << endl; exit(1);
            }
        }

        void expressao_simples_(){
            // cout << "estou em expressao_simples_" << endl;
            switch(this->token){
                case(R_C_BRACKET): break;
                case(R_S_BRACKET): break;
                case(SEMICOLON): break;
                case(EQUAL): break;
                case(COMMA): break;
                case(ATE): break;
                case(ENTAO): break;
                case(FACA): break;
                case(PASSO): break;
                case(DIFFERENT): break;
                case(LESS): break;
                case(LEQ): break;
                case(GREATER): break;
                case(GEQ): break;
                case(PLUS): eat(PLUS); termo(); expressao_simples_(); break;
                case(MINUS): eat(MINUS); termo(); expressao_simples_(); break;
                case(OU): eat(OU); termo(); expressao_simples_(); break;


                default: ERRO_SINTATICO; // cout << "erro em expressao_simples_" << endl; exit(1);
            }
        }

        void termo_(){
            // cout << "estou em termo_" << endl;
            switch(this->token){
                case(SEMICOLON): break;
                case(R_C_BRACKET): break;
                case(R_S_BRACKET): break;
                case(EQUAL): break;
                case(COMMA): break;
                case(ENTAO): break;
                case(FACA): break;
                case(ATE): break;
                case(PASSO): break;
                case(PLUS): break;
                case(MINUS): break;
                case(DIFFERENT): break;
                case(LESS): break;
                case(LEQ): break;
                case(GREATER): break;
                case(GEQ): break;
                case(OU): break;
                case(MULT): eat(MULT); fator(); termo_(); break;
                case(DIVISION): eat(DIVISION); fator(); termo_(); break;
                case(DIV): eat(DIV); fator(); termo_(); break;
                case(E): eat(E); fator(); termo_(); break;

                default: ERRO_SINTATICO; // cout << "erro em termo_" << endl; exit(1);
            }
        }


    public:
        // Automaton(int initial_state);
        // void Analyze(string input);
         

        Automaton(int initial_state){
            this->initial_state = initial_state;
            this->current_index = 0;
            this->input = input;
        }

        void Analyze(string input){
            this->current_index = 0;
            this->input = input;
            this->line_no = 1;
            this->col_no = 1;
            //cout << "Analisando: " << input << endl;

            advance();
            //call the program function
            programa();

        }

};


int main(){

    int current_state = 1, last_final_index, current_final_index;

    string input; char symbol;
    while(cin.get(symbol)){
        input += symbol;
    }
    input += '\0';

    Automaton automato(1);
    automato.Analyze(input);

    return 0;
}