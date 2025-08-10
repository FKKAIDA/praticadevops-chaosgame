#ifndef TOKEN_H
#define TOKEN_H
#include <iostream>
#include <string>
#include <map>

enum TipoToken {ERRO, FIM, INTEIRO, REAL, LN, CR, 
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, //+ - * /
    OP_EQ, OP_NE, OP_LT, OP_GT, OP_LET, OP_GET, //== != < > <= >=
    OP_AND, OP_OR, OP_NOT, //&& || !
    DEL_APAR, DEL_FPAR, DEL_ACOL, DEL_FCOL,DEL_ACHA,DEL_FCHA, DEL_COL, DEL_SMC, DEL_COM,//DELIMITADORES, Abre PARentêses, Fecha PARentêses, COLchetes e CHAves...
    CADEIA, CONFIGURACOES, SALTO, ITERACOES, TELA, ORIGEM, RESOLUCAO, POLIGONO, INSCRITO, CENTRO, RAIO, NRO, MANUAL, VERTICE, 
    INSTRUCOES, TODOS, ANTERIOR, INCLUIR, EXCLUIR, IF, ELSE
};

extern std::map<int, std::string> Token_EnumToString;

class Token{
    public:
        Token(TipoToken tipo, std::string lexema);
        TipoToken tipo;
        std::string lexema;
        
};

std::ostream& operator<<(std::ostream& stream, const Token& T);
bool operator==(const Token& L, const Token& R);

#endif