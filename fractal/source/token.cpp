#include "token.h"
#include <string>



Token::Token(TipoToken tipo, std::string lexema){
    this->tipo = tipo;
    this->lexema = lexema;
}

std::map<int, std::string> Token_EnumToString = {
    {TipoToken::ERRO, "ERRO"},
    {TipoToken::FIM, "FIM"},
    {TipoToken::INTEIRO,"INTEIRO"},
    {TipoToken::REAL,"REAL"},
    {TipoToken::LN,"LINE FEED"},
    {TipoToken::CR,"CARRIAGE RETURN"},
    {TipoToken::OP_ADD, "OP_ADD +"},
    {TipoToken::OP_SUB, "OP_SUB -"},
    {TipoToken::OP_MUL, "OP_MUL *"},
    {TipoToken::OP_DIV, "OP_DIV /"},
    {TipoToken::OP_EQ, "OP_EQ =="},
    {TipoToken::OP_NE, "OP_NE !="},
    {TipoToken::OP_LT, "OP_LT <"},
    {TipoToken::OP_GT, "OP_GT >"},
    {TipoToken::OP_LET, "OP_LET <="},
    {TipoToken::OP_GET, "OP_GET >="},
    {TipoToken::OP_AND, "OP_AND &&"},
    {TipoToken::OP_OR, "OP_OR ||"},
    {TipoToken::OP_NOT, "OP_NOT !"},
    {TipoToken::DEL_APAR, "DEL_APAR ("},
    {TipoToken::DEL_FPAR, "DEL_FPAR )"},
    {TipoToken::DEL_ACOL, "DEL_ACOL ["},
    {TipoToken::DEL_FCOL, "DEL_FCOL ]"},
    {TipoToken::DEL_ACHA, "DEL_ACHA {"},
    {TipoToken::DEL_FCHA, "DEL_FCHA }"},
    {TipoToken::DEL_SMC, "DEL_SMC ;"},
    {TipoToken::DEL_COM, "DEL_COM ,"},
    {TipoToken::CADEIA, "CADEIA"},
    {TipoToken::CONFIGURACOES, "CONFIGURACOES"},
    {TipoToken::SALTO, "SALTO"},
    {TipoToken::ITERACOES, "ITERACOES"},
    {TipoToken::TELA, "TELA"},
    {TipoToken::ORIGEM, "ORIGEM"},
    {TipoToken::RESOLUCAO, "RESOLUCAO"},
    {TipoToken::POLIGONO, "POLIGONO"},
    {TipoToken::INSCRITO, "INSCRITO"},
    {TipoToken::CENTRO, "CENTRO"},
    {TipoToken::RAIO, "RAIO"}, 
    {TipoToken::NRO, "NRO"},
    {TipoToken::MANUAL, "VERTICE"},
    {TipoToken::INSTRUCOES, "INSTRUCOES"},
    {TipoToken::TODOS, "TODOS"},
    {TipoToken::ANTERIOR, "ANTERIOR"},
    {TipoToken::INCLUIR, "INCLUIR"},
    {TipoToken::EXCLUIR, "EXCLUIR"},
    {TipoToken::IF, "IF"},
    {TipoToken::ELSE, "ELSE"}
};

std::ostream& operator<<(std::ostream& stream, const Token& T){

    stream << "<" << Token_EnumToString[T.tipo] << "," << T.lexema << ">";
    return stream;
}

bool operator==(const Token& L, const Token& R){
    return (L.tipo == R.tipo) && (L.lexema == R.lexema);
}
