#ifndef GERADOR_H
#define GERADOR_H
#include "token.h"
#include <fstream>
#include <vector>
#include <cmath>
#include <numeric>

class Gerador{
    public:
        Gerador(std::vector<Token*> tokenlist);
        bool gerar();
        int ln_count();
        std::string error;

    private:
        std::vector<Token*> tokenlist;
        Token* lookahead(int k);
        bool match(TipoToken T);
        int p, indentation, nro, w, h, origem_x, origem_y, iter;
        std::vector<int> poligono_aux;
        bool anterior;
        std::ofstream writer;


        bool CONFIGURACOES();
        bool POLIGONO();
        bool ITERACOES();
        bool TELA();
        bool SALTO();

        int EXPR_INT_1();
        int EXPR_INT_1_AUX(int lhs);
        int EXPR_INT_2();
        int EXPR_INT_2_AUX(int lhs);
        int EXPR_INT_3();

        bool INSTRUCOES();
        bool COMMANDOS_INSTRUCOES();
        bool INCLUIR_EXCLUIR();
        bool IF();
};

#endif