#ifndef ALEXICO_H
#define ALEXICO_H
#include "leitor.h"
#include "token.h"
#include <vector>

class ALexico {
    public:
        ALexico(std::string path);
        Token* getnexttoken();
        std::vector<Token*> gettokenlist();
        int ln_count();
    private:
        int ln_c = 0;
        Leitor L;
        Token* fim();
        Token* linefeed();
        Token* numero();
        Token* operacao();
        Token* delimitador();
        Token* palavrachave();
        void skip();

};

#endif