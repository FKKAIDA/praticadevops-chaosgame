#include "asintatico.h"

extern std::map<int, std::string> Token_EnumToString;

ASintatico::ASintatico(std::vector<Token*> tokenlist){
    this->tokenlist = tokenlist;
    p = 0;
    while(tokenlist[p]->tipo == TipoToken::LN){
        p++;
    }
}

int ASintatico::ln_count(){
    int count = 0;
    for(int i = 0; i < p; i++){
        if(tokenlist[i]->tipo == TipoToken::LN)
            count++;
    }
    return count + 1;
}

Token* ASintatico::lookahead(int k){
    int inc = 0;
    while(k > 0 && tokenlist[p + inc]->tipo != TipoToken::FIM){
        inc ++;
        if(tokenlist[p + inc]->tipo != TipoToken::LN){
            k--;
        }
    }
    return tokenlist[p + inc];
}

bool ASintatico::match(TipoToken TT){
    if(tokenlist[p]->tipo == TT){
        if(p < tokenlist.size() -1 ){
            p++;
            while(tokenlist[p]->tipo == TipoToken::LN){
                p++;
            }
        }
        return true;
    }else{
        error = "Erro Sintatico. Esperado: " + Token_EnumToString[TT] + " Recebido: " +  Token_EnumToString[tokenlist[p]->tipo] + " p = " + std::to_string(p);
        return false;
    }
}

// S -> CONFIGURACOES INSTRUCOES $
bool ASintatico::Start(){
    if(tokenlist.empty())
        return false;

    if(!CONFIGURACOES()){
        return false;
    }

    if(!INSTRUCOES()){
        return false;
    }

    if(!match(TipoToken::FIM)){
        return false;
    }
    return true;
}


//CONFIGURACOES -> configuracoes { LISTA_COMANDOS_CONFIGURACOES }
bool ASintatico::CONFIGURACOES(){
    if(!match(TipoToken::CONFIGURACOES)){
        return false;
    }

    if(!match(TipoToken::DEL_ACHA)){
        return false;
    }
    if(!LISTA_COMANDOS_CONFIGURACOES()){
        return false;
    }
    if(!match(TipoToken::DEL_FCHA)){
        return false;
    }
    return true;
}


//LISTA_COMANDOS_CONFIGURACOES -> COMANDO_CONFIGURACOES LISTA_COMANDOS_CONFIGURACOES | <>
//COMANDO_CONFIGURACOES -> ITERACOES | TELA | POLIGONO
//Foi feito uma modificacao nesta regra para que cada comando seja executado uma única vez
//No return eh feito o teste dos comandos obrigatorios
bool ASintatico::LISTA_COMANDOS_CONFIGURACOES(){
    bool iteracoes = false, tela = false, poligono = false, salto = false;
    while(lookahead(0)->tipo == TipoToken::SALTO ||
          lookahead(0)->tipo == TipoToken::ITERACOES || 
          lookahead(0)->tipo == TipoToken::TELA || 
          lookahead(0)->tipo == TipoToken::POLIGONO){
        

        switch(lookahead(0)->tipo){
            case TipoToken::SALTO:
                if(salto){
                    error = "ERRO: SALTO ja declarado";
                    return false;
                }else{
                    salto = true;
                    if(!SALTO()){
                        return false;
                    }
                }
            break;
            case TipoToken::ITERACOES:
                if(iteracoes){
                    error = "ERRO: ITERACOES ja declarado";
                    return false;
                }else{
                    iteracoes = true;
                    if(!ITERACOES())
                        return false;
                }
            break;
            case TipoToken::TELA:
                if(tela){
                    error = "ERRO: TELA ja declarado";
                    return false;
                }else{
                    tela = true;
                    if(!TELA())
                        return false;
                }
            break;
            case TipoToken::POLIGONO:
                if(poligono){
                    error = "ERRO: POLIGONO ja declarado";
                    return false;
                }else{
                    poligono = true;
                    if(!POLIGONO())
                        return false;
                }
            break;
            default:
                error = "ERRO: Esperado SALTO | ITERACOES | TELA |POLIGONO";
                return false;
        }
    }

    if(!(iteracoes && tela && poligono && salto)){
        error = "ERRO: Eh necessario declar SALTO, ITERACOES, TELA E POLIGONO";
        return false;
    }else{
        return true;
    }
}

bool ASintatico::SALTO(){
    if(!match(TipoToken::SALTO)){
        return false;
    }

    switch(lookahead(0)->tipo){
        case TipoToken::INTEIRO:
            if(!match(TipoToken::INTEIRO)){
                return false;
            }
            if(!match(TipoToken::INTEIRO)){
                return false;
            }

            return true;
        break;
        case TipoToken::REAL:
            if(!match(TipoToken::REAL)){
                return false;
            }
            return true;
        break;
        default:
            error = "ERRO: Esperado SALTO (INTEIRO INTEIRO | REAL)";
            return false; 
    }
    error = "ERRO: Esperado SALTO (INTEIRO INTEIRO | REAL)";
    return false;
}

bool ASintatico::ITERACOES(){

    if(!(match(TipoToken::ITERACOES) && match(TipoToken::INTEIRO))){
        error = "ERRO: Esperado ITERACOES INTEIRO";
        return false;
    }{
        return true;
    }
}

bool ASintatico::TELA(){

    if(!(match(TipoToken::TELA) && 
           match(TipoToken::DEL_ACHA) &&
           match(TipoToken::ORIGEM) &&
           match(TipoToken::INTEIRO) &&
           match(TipoToken::INTEIRO) &&
           match(TipoToken::RESOLUCAO) &&
           match(TipoToken::INTEIRO) &&
           match(TipoToken::INTEIRO) &&
           match(TipoToken::DEL_FCHA))){

            error = "ERRO: Esperado TELA DEL_ACHA ORIGEM INTEIRO INTEIRO RESOLUCAO INTEIRO INTEIRO DEL_FCHA";
            return false;
    }else{
        return true;
    }
    
}

//POLIGONO -> poligono { INSCRITO | MANUAL }
bool ASintatico::POLIGONO(){
    if(!match(TipoToken::POLIGONO))
        return false;

    if(!match(TipoToken::DEL_ACHA))
        return false;

    bool aux;
    if(lookahead(0)->tipo == TipoToken::INSCRITO){
        if(!INSCRITO())
            return false;
    }else if(lookahead(0)->tipo == TipoToken::MANUAL){
        if(!MANUAL())
            return false;
    }else{
        error = "ERRO: Esperado INSCRITO | MANUAL";
        return false;
    }

    if(!match(TipoToken::DEL_FCHA))
        return false;
    
    return true;
}

bool ASintatico::INSCRITO(){
    
    if(!( match(TipoToken::INSCRITO) &&
           match(TipoToken::CENTRO) &&
           match(TipoToken::INTEIRO) &&
           match(TipoToken::INTEIRO) &&
           match(TipoToken::RAIO) &&
           match(TipoToken::INTEIRO) &&
           match(TipoToken::NRO) &&
           match(TipoToken::INTEIRO))){
            error = "ERRO: Esperado ISCRITO MANUAL CENTRO INTEIRO INTEIRO RAIO INTEIRO NRO INTEIRO";
            return false;
    }
    return true;

}


//MANUAL ->manual (vertice inteiro inteiro (inteiro | <>))+
bool ASintatico::MANUAL(){
    if(!match(TipoToken::MANUAL)){
        return false;
    }
    
    bool once = false;
    while(lookahead(0)->tipo == TipoToken::VERTICE){
        once = true;
        if(!match(TipoToken::VERTICE))
            return false;
        if(!match(TipoToken::INTEIRO))
            return false;
        if(!match(TipoToken::INTEIRO))
            return false;
        if(lookahead(0)->tipo == TipoToken::INTEIRO){
            match(TipoToken::INTEIRO);
        }
    }

    if(!once){
        error = "ERRO: Esperado MANUAL (VERTICE INTEIRO INTEIRO)+";
        return false;
    }

    return true;
}

bool ASintatico::INSTRUCOES(){
    if(!match(TipoToken::INSTRUCOES)){
        return false;
    }

    if(!match(TipoToken::DEL_ACHA)){
        return false;
    }
    
    if(!LISTA_COMANDOS_INSTRUCOES()){
        return false;
    }

    if(!match(TipoToken::DEL_FCHA)){
        return false;
    }

    return true;
}

// LISTA_COMANDOS_INSTRUCOES -> COMANDO_INSTRUCOES LISTA_COMANDOS_INSTRUCOES | <>
bool ASintatico::LISTA_COMANDOS_INSTRUCOES(){
    while(lookahead(0)->tipo == TipoToken::INCLUIR ||
          lookahead(0)->tipo == TipoToken::EXCLUIR ||
          lookahead(0)->tipo == TipoToken::IF){
        
        if(!COMANDO_INSTRUCOES()){
            return false;
        }
    }
    return true;
}

//COMANDO_INSTRUCOES -> INCLUIR | EXCLUIR | IF
bool ASintatico::COMANDO_INSTRUCOES(){
    switch(lookahead(0)->tipo){
        case TipoToken::INCLUIR:
        case TipoToken::EXCLUIR:
            return INCLUIR_EXCLUIR();
        break;
        case TipoToken::IF:
            return IF();
        break;
        default:
            error = "ERRO: Esperado INCLUIR | EXCLUIR | IF";
            return false;
    }

    return false;
}

//INCLUIR -> incluir VERTICE | LISTA_VERTICES
bool ASintatico::INCLUIR_EXCLUIR(){
    switch(lookahead(0)->tipo){
        case TipoToken::INCLUIR:
            if(!match(TipoToken::INCLUIR)){
                return false;
            }
        break;
        case TipoToken::EXCLUIR:
            if(!match(TipoToken::EXCLUIR)){
                return false;
            }
        break;
        default:
            return false;
    }
    

    if(lookahead(0)->tipo == TipoToken::TODOS){
        if(!match(TipoToken::TODOS)){
            return false;
        }
        return true;
    }


    if(!VERTICE()){
        return false;
    }
    if(lookahead(0)->tipo == TipoToken::OP_ADD){
        if(!match(TipoToken::OP_ADD)){
            return false;
        }
        if(!LISTA_INT()){
            return false;
        }

    }else if(lookahead(0)->tipo == TipoToken::OP_SUB){
        if(!match(TipoToken::OP_SUB)){
            return false;
        }
        if(!LISTA_INT()){
            return false;
        }
    }

    return true;
}



//IF -> if (EXPR_BOOL_1) {instrucoes}
bool ASintatico::IF(){
    if(!match(TipoToken::IF)){
        return false;
    }

    if(!match(TipoToken::DEL_APAR)){
        return false;
    }
    
    if(!EXPR_BOOL_1()){
        return false;
    }

    if(!match(TipoToken::DEL_FPAR)){
        return false;
    }

    if(!match(TipoToken::DEL_ACHA)){
        return false;
    }
    
    if(!LISTA_COMANDOS_INSTRUCOES()){
        return false;
    }

    if(!match(TipoToken::DEL_FCHA)){
        return false;
    }

    if(lookahead(0)->tipo == TipoToken::ELSE){
        if(!match(TipoToken::ELSE)){
            return false;
        }

        switch(lookahead(0)->tipo){
            case TipoToken::DEL_ACHA:
            if(!match(TipoToken::DEL_ACHA)){
                return false;
            }
            
            if(!LISTA_COMANDOS_INSTRUCOES()){
                return false;
            }

            if(!match(TipoToken::DEL_FCHA)){
                return false;
            }
            break;
            case TipoToken::IF:
                if(!IF()){
                    return false;
                }
            break;
            default:
                return false;
        }
    }


    return true;
}

//VERTICE -> (vertice | anterior) ( EXPR_INT_1 ) ((+ | -) EXPR_INT_1) | <>
bool ASintatico::VERTICE(){
    switch(lookahead(0)->tipo){
        case TipoToken::VERTICE:
            if(!match(TipoToken::VERTICE)){
                return false;
            }
        break;
        case TipoToken::ANTERIOR:
            if(!match(TipoToken::ANTERIOR)){
                return false;
            }
        break;
        default:
            error = "ERRO: Esperado TODOS | VERTICE | ANTERIOR";
            return false;
    }
    if(!match(TipoToken::DEL_APAR)){
        return false;
    }

    if(!EXPR_INT_1()){
        return false;
    }

    if(!match(TipoToken::DEL_FPAR)){
        return false;
    }

    if(lookahead(0)->tipo == TipoToken::OP_ADD && (lookahead(1)->tipo == TipoToken::INTEIRO || lookahead(1)->tipo == TipoToken::DEL_APAR)){
        if(!match(TipoToken::OP_ADD)){
            return false;
        }

        if(!EXPR_INT_1()){
            return false;
        }
    } else if(lookahead(0)->tipo == TipoToken::OP_SUB  && (lookahead(1)->tipo == TipoToken::INTEIRO || lookahead(1)->tipo == TipoToken::DEL_APAR)){
        if(!match(TipoToken::OP_SUB)){
            return false;
        }

        if(!EXPR_INT_1()){
            return false;
        }
    } 

    return true;
}

bool ASintatico::LISTA_INT(){
    if(!match(TipoToken::DEL_ACOL)){
        return false;
    }

    if(!EXPR_INT_1()){
        return false;
    }

    while(lookahead(0)->tipo == TipoToken::DEL_COM){
        if(!match(TipoToken::DEL_COM)){
            return false;
        }
        if(!EXPR_INT_1()){
            return false;
        }
    }

    if(!match(TipoToken::DEL_FCOL)){
        return false;
    }

    return true;
}

bool ASintatico::EXPR_INT_1(){


    if(!EXPR_INT_2()){
        return false;
    }

    if(lookahead(1)->tipo == TipoToken::INTEIRO || lookahead(1)->tipo == TipoToken::DEL_APAR)
        if(!EXPR_INT_1_AUX()){
            return false;
        }

    return true;
}

bool ASintatico::EXPR_INT_1_AUX(){
    if(lookahead(0)->tipo == TipoToken::OP_ADD || lookahead(0)->tipo == TipoToken::OP_SUB){
        if(!OP_INT_1()){
            return false;
        }

        if(!EXPR_INT_2()){
            return false;
        }

        if(!EXPR_INT_1_AUX()){
            return false;
        }

        return true;
    }

    return true;
}

bool ASintatico::OP_INT_1(){
    switch(lookahead(0)->tipo){
        case TipoToken::OP_ADD:
            return match(TipoToken::OP_ADD);
        break;
        case TipoToken::OP_SUB:
            return match(TipoToken::OP_SUB);
        break;
        default:
            return false;
    }
}

bool ASintatico::EXPR_INT_2(){
    if(!EXPR_INT_3()){
        return false;
    }

    if(!EXPR_INT_2_AUX()){
        return false;
    }

    return true;    
}

bool ASintatico::EXPR_INT_2_AUX(){
    if(lookahead(0)->tipo == TipoToken::OP_MUL || lookahead(0)->tipo == TipoToken::OP_DIV){
        if(!OP_INT_2()){
            return false;
        }

        if(!EXPR_INT_3()){
            return false;
        }

        if(!EXPR_INT_2_AUX()){
            return false;
        }

        return true;
    }

    return true;
}

bool ASintatico::OP_INT_2(){
    switch(lookahead(0)->tipo){
        case TipoToken::OP_MUL:
            return match(TipoToken::OP_MUL);
        break;
        case TipoToken::OP_DIV:
            return match(TipoToken::OP_DIV);
        break;
        default:
            return false;
    }

    return false;
}

bool ASintatico::EXPR_INT_3(){
    switch(lookahead(0)->tipo){
        case TipoToken::DEL_APAR:
            if(!match(TipoToken::DEL_APAR)){
                return false;
            }

            if(!EXPR_INT_1()){
                return false;
            }

            if(!match(TipoToken::DEL_FPAR)){
                return false;
            }

            return true;
        break;
        case TipoToken::INTEIRO:
            return match(TipoToken::INTEIRO);
        break;
        default:
            return false;
    }

    return false;
}

bool ASintatico::EXPR_BOOL_1(){
    if(!EXPR_BOOL_2()){
        return false;
    }
    
    if(!EXPR_BOOL_1_AUX()){
        return false;
    }

    return true;
}

bool ASintatico::EXPR_BOOL_1_AUX(){
    if(lookahead(0)->tipo == TipoToken::OP_OR){
        if(!match(TipoToken::OP_OR)){
            return false;
        }

        if(!EXPR_BOOL_2()){
            return false;
        }

        if(!EXPR_BOOL_1_AUX()){
            return false;
        }
    }

    return true;
}

bool ASintatico::EXPR_BOOL_2(){
    if(!EXPR_BOOL_3()){
        return false;
    }

    if(!EXPR_BOOL_2_AUX()){
        return false;
    }

    return true;
}

bool ASintatico::EXPR_BOOL_2_AUX(){
    if(lookahead(0)->tipo == TipoToken::OP_AND){
        if(!match(TipoToken::OP_AND)){
            return false;
        }

        if(!EXPR_BOOL_3()){
            return false;
        }

        if(!EXPR_BOOL_2_AUX()){
            return false;
        }
    }

    return true;
}

bool ASintatico::EXPR_BOOL_3(){
    if(lookahead(0)->tipo == OP_NOT){
        if(!match(TipoToken::OP_NOT)){
            return false;
        }
    }

    if(!EXPR_BOOL_4()){
        return false;
    }

    return true;
}

bool ASintatico::EXPR_BOOL_4(){
    switch(lookahead(0)->tipo){
        case TipoToken::DEL_APAR:
            if(!match(TipoToken::DEL_APAR)){
                return false;
            }
            if(!EXPR_BOOL_1()){
                return false;
            }
            if(!match(TipoToken::DEL_FPAR)){
                return false;
            }
        break;
        case TipoToken::VERTICE:
        case TipoToken::ANTERIOR:
            if(!VERTICE()){
                return false;
            }
            if(!OP_REL_VERTICE()){
                return false;
            }
            if(!VERTICE()){
                return false;
            }
        break;
        default:
            return false;
    }

    return true;
}

bool ASintatico::OP_REL_VERTICE(){
    switch(lookahead(0)->tipo){
        case TipoToken::OP_EQ:
            if(!match(TipoToken::OP_EQ)){
                return false;
            }
        break;
        case TipoToken::OP_NE:
            if(!match(TipoToken::OP_NE)){
                return false;
            }
        break;
        default:
            return false;
    }
    return true;
}