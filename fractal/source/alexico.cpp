#include "alexico.h"

ALexico::ALexico(std::string path) : L(path){
}

std::vector<Token*> ALexico::gettokenlist(){
    std::vector<Token*> tokenlist;
    Token* T;
    do{
        T = getnexttoken();
        if(T->tipo != TipoToken::CR)
            tokenlist.push_back(T);
    }while(tokenlist.back()->tipo != TipoToken::FIM && tokenlist.back()->tipo != TipoToken::ERRO);

    return tokenlist;
}

Token* ALexico::getnexttoken(){
    
    Token* T;

    skip();
    L.confirmar();

    T = fim();
    if(T != nullptr){
        L.confirmar();
        return T;
    }else{
        L.reiniciar();
    }

    T = linefeed();
    if(T != nullptr){
        L.confirmar();
        return T;
    }else{
        L.reiniciar();
    }

    T = numero();
    if(T != nullptr){
        L.confirmar();
        return T;
    }else{
        L.reiniciar();
    }

    T = operacao();
    if(T != nullptr){
        L.confirmar();
        return T;
    }else{
        L.reiniciar();
    }

    T = delimitador();
    if(T != nullptr){
        L.confirmar();
        return T;
    }else{
        L.reiniciar();
    }

    T = palavrachave();
    if(T != nullptr){
        L.confirmar();
        return T;
    }else{
        L.reiniciar();
    }
    return new Token(TipoToken::ERRO, std::string{char(L.readnext())});
}

int ALexico::ln_count(){
    return ln_c + 1;
}

Token* ALexico::fim(){
    int c = L.readnext();
    //EOF = -1 NUL = 0
    if(c == -1 || c == 0){
        return new Token(TipoToken::FIM, L.getlexama());
    }else{
        return nullptr;
    }
}

Token* ALexico::linefeed(){
    int c = L.readnext();
    //linefeed or carriage return (13)
    switch(c){
        case('\n'):
            this->ln_c++;
            return new Token(TipoToken::LN, L.getlexama());
        break;
        case(13):
            return new Token(TipoToken::CR, L.getlexama());
        break;
        default:
            return nullptr;
    }
}

//48..57 -> 0..9
Token* ALexico::numero(){
    int c, state = 0;
    
    while(true){

        c = L.readnext();

        switch(state){
            case 0:
                if((c >= '0' && c<= '9')){
                    state = 1;
                }else if((c == '+') || (c == '-')){
                    state = 3;
                }else{
                    return nullptr;
                }
            break;
            case 1:
                if(c == '.'){
                    state = 2;
                }else if(!(c >= '0' && c<= '9')   ){
                    L.dec();
                    return new Token(TipoToken::INTEIRO, L.getlexama());
                }
            break;
            case 2:
                if(!(c >= '0' && c <= '9')){
                    L.dec();
                    return new Token(TipoToken::REAL, L.getlexama());
                }
            break;
            case 3:
                if((c >= '0' && c<= '9')){
                    state = 1;
                }else{
                    return nullptr;
                }
            break;
        }
    }
}

Token* ALexico::operacao(){
    char c = L.readnext(), d;

    switch(c){
        case '+':
            return new Token(TipoToken::OP_ADD, L.getlexama());
        break;
        case '-':
            return new Token(TipoToken::OP_SUB, L.getlexama());
        break;
        case '*':
            return new Token(TipoToken::OP_MUL, L.getlexama());
        break;
        case '/':
            return new Token(TipoToken::OP_DIV, L.getlexama());
        break;
        case '<':
            d = L.readnext();
            if(d == '='){
                return new Token(TipoToken::OP_LET, L.getlexama());
            }else{
                L.dec();
                return new Token(TipoToken::OP_LT, L.getlexama());
            }
        break;
        case '>':
            d = L.readnext();
            if(d == '='){
                return new Token(TipoToken::OP_GET, L.getlexama());
            }else{
                L.dec();
                return new Token(TipoToken::OP_GT, L.getlexama());
            }
        break;
        case '=':
            d = L.readnext();
            if(d == '='){
                return new Token(TipoToken::OP_EQ, L.getlexama());
            }
        break;
        case '!':
            d = L.readnext();
            if(d == '='){
                return new Token(TipoToken::OP_NE, L.getlexama());
            }else{
                L.dec();
                return new Token(TipoToken::OP_NOT, L.getlexama());
            }
        break;
        case '&':
            d = L.readnext();
            if(d == '&')
                return new Token(TipoToken::OP_AND, L.getlexama());
        break;
        case '|':
            d = L.readnext();
            if(d == '|')
                return new Token(TipoToken::OP_OR, L.getlexama());
        break;
    }


    return nullptr;
}

Token* ALexico::delimitador(){
    int c = L.readnext();

    switch(c){
        case '(':
            return new Token(TipoToken::DEL_APAR, L.getlexama());
        break;
        case ')':
            return new Token(TipoToken::DEL_FPAR, L.getlexama());
        break;
        case '[':
            return new Token(TipoToken::DEL_ACOL, L.getlexama());
        break;
        case ']':
            return new Token(TipoToken::DEL_FCOL, L.getlexama());
        break;
        case '{':
            return new Token(TipoToken::DEL_ACHA, L.getlexama());
        break;
        case '}':
            return new Token(TipoToken::DEL_FCHA, L.getlexama());
        break;
        case ':':
            return new Token(TipoToken::DEL_COL, L.getlexama());
        break;
        case ';':
            return new Token(TipoToken::DEL_SMC, L.getlexama());
        break;
        case ',':
            return new Token(TipoToken::DEL_COM, L.getlexama());
        break;
        default:
            return nullptr;
    }
}

void ALexico::skip(){
    int c = L.readnext();
    while(c == ' ' || c == '\t' || c == 13){
        c = L.readnext();
    }
    L.dec();
}

Token* ALexico::palavrachave(){
    std::string s = "";
    char c;
    int state = 0;
    
    while(true){
        c = L.readnext();
        switch(state){
            case 0:
                if(!(c == '_' || ( c>='a' && c<='z' ) || (c >= 'A' && c <= 'Z') )){
                    return nullptr;
                }else{
                    s += c;
                    state = 1;
                }
            break;
            case(1):
            if(!(c == '_' || ( c>='a' && c<='z' ) || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))){
                L.dec();
                if(s == "CONFIGURACOES"){ 
                    return new Token(TipoToken::CONFIGURACOES, L.getlexama());
                }
                if(s == "SALTO") 
                    return new Token(TipoToken::SALTO, L.getlexama());
                if(s == "TELA")
                    return new Token(TipoToken::TELA, L.getlexama());
                if(s == "ORIGEM") 
                    return new Token(TipoToken::ORIGEM, L.getlexama());
                if(s == "ITERACOES") 
                    return new Token(TipoToken::ITERACOES, L.getlexama());
                if(s == "RESOLUCAO") 
                    return new Token(TipoToken::RESOLUCAO, L.getlexama());
                if(s == "POLIGONO") 
                    return new Token(TipoToken::POLIGONO, L.getlexama());
                if(s == "INSCRITO") 
                    return new Token(TipoToken::INSCRITO, L.getlexama());
                if(s == "CENTRO") 
                    return new Token(TipoToken::CENTRO, L.getlexama());
                if(s == "RAIO") 
                    return new Token(TipoToken::RAIO, L.getlexama());
                if(s == "NRO") 
                    return new Token(TipoToken::NRO, L.getlexama());
                if(s == "MANUAL") 
                    return new Token(TipoToken::MANUAL, L.getlexama());
                if(s == "VERTICE") 
                    return new Token(TipoToken::VERTICE, L.getlexama());
                if(s == "INSTRUCOES")
                    return new Token(TipoToken::INSTRUCOES, L.getlexama());
                if(s == "TODOS")
                    return new Token(TipoToken::TODOS, L.getlexama());
                if(s == "ANTERIOR")
                    return new Token(TipoToken::ANTERIOR, L.getlexama());
                if(s == "INCLUIR") 
                    return new Token(TipoToken::INCLUIR, L.getlexama());
                if(s == "EXCLUIR") 
                    return new Token(TipoToken::EXCLUIR, L.getlexama());
                if(s == "IF") 
                    return new Token(TipoToken::IF, L.getlexama());
                if(s == "ELSE") 
                    return new Token(TipoToken::ELSE, L.getlexama());
                return nullptr;
            }else{
                s += c;
            }
            break;
        }
    }

    return nullptr;
}