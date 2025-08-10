#ifndef ASINTATICO_H
#define ASINTATICO_H
#include "token.h"
#include <vector>


class ASintatico {
    public:
        ASintatico(std::vector<Token*> tokenlist);
        bool Start();
        int ln_count();
        int p; //tokenlist index
        std::string error = "";
           private:
        std::vector<Token*> tokenlist;

        Token* lookahead(int k);
        bool match(TipoToken T);

        bool CONFIGURACOES();
        bool INSTRUCOES();
        bool LISTA_COMANDOS_CONFIGURACOES();
        //bool COMMANDO_CONFIGURACOES();
        bool SALTO();
        bool TELA();
        bool ITERACOES();
        bool POLIGONO();
        bool INSCRITO();
        bool MANUAL();

        bool LISTA_COMANDOS_INSTRUCOES();
        bool COMANDO_INSTRUCOES();
        bool INCLUIR_EXCLUIR();
        bool IF();

        //
        bool VERTICE();
        bool LISTA_INT();
        
        bool EXPR_INT_1();
        bool EXPR_INT_1_AUX();
        bool OP_INT_1();
        bool EXPR_INT_2();
        bool EXPR_INT_2_AUX();
        bool OP_INT_2();
        bool EXPR_INT_3();

        bool EXPR_BOOL_1();
        bool EXPR_BOOL_1_AUX();
        bool EXPR_BOOL_2();
        bool EXPR_BOOL_2_AUX();
        bool EXPR_BOOL_3();
        bool EXPR_BOOL_4();
        bool OP_REL_VERTICE();
};

#endif