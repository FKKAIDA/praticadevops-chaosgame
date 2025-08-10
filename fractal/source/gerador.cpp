#include "gerador.h"

Gerador::Gerador(std::vector<Token*> tokenlist){
    this->tokenlist = tokenlist;
    this->p = 0;
    while(tokenlist[p]->tipo == TipoToken::LN){
        p++;
    }
    this->anterior = false;
    this->indentation = 0;
    writer.open("code.cpp");
}

int Gerador::ln_count(){
    int count = 0;
    for(int i = 0; i < p; i++){
        if(tokenlist[i]->tipo == TipoToken::LN)
            count++;
    }
    return count + 1;
}

Token* Gerador::lookahead(int k){
    int inc = 0;
    while(k > 0 && tokenlist[p + inc]->tipo != TipoToken::FIM){
        inc ++;
        if(tokenlist[p + inc]->tipo != TipoToken::LN){
            k--;
        }
    }
    return tokenlist[p + inc];
}

bool Gerador::match(TipoToken TT){
    if(tokenlist[p]->tipo == TT){
        if(p < tokenlist.size() -1){
            p++;
            while(tokenlist[p]->tipo == TipoToken::LN){
                p++;
            }
        }
        return true;
    }else{
        return false;
    }
}


bool Gerador::gerar(){
    writer << "#define STB_IMAGE_WRITE_IMPLEMENTATION\n";
    writer << "#include \"stb_image_write.h\"\n";
    writer << "#include<random>\n";
    writer << "#include<cstring>\n";
    writer << "#define L_CSHIFT(number, shift, size, mask) (mask & ((number << (shift)) | (number >> (size - shift))))\n\n";;

    CONFIGURACOES();


    int p_aux = p, max = 0, eval, vertice;

    while(lookahead(0)->tipo != TipoToken::FIM){
        if(lookahead(0)->tipo == TipoToken::ANTERIOR){
            match(TipoToken::ANTERIOR);
            match(TipoToken::DEL_APAR);
            eval = EXPR_INT_1();
            if(eval <= 0){
                error = "ERRO: Para acessar vertices anteriores eh necessario usar um indice natural. Indice acessado: " + std::to_string(eval);
                return false;
            }
             
            if(eval > max){
                max = eval;
            }
        }else if(lookahead(0)->tipo == TipoToken::VERTICE){
            match(TipoToken::VERTICE);
            match(TipoToken::DEL_APAR);
            vertice = EXPR_INT_1();
            if(vertice < 0 || vertice >= this->nro){
                error = "ERRO: Faixa de indices de 0 a " +   std::to_string(this->nro - 1) + ". Indice acessado: " + std::to_string(vertice);
                return false;
            }
        }else{
            match(lookahead(0)->tipo);
        }
    }

    p = p_aux;

    if(max > 0){
        max++;
        anterior = true;
        writer << std::string(indentation, '\t') << "int asize = " << max << ", head = 0, ANTERIOR[" << max << "] = {";

        for(int i = 0; i < max ; i++){
            writer << "0";
            if(i < max - 1){
                writer << ", ";
            }
        }
        writer << "};\n\n";
    }


    writer << std::string(indentation, '\t') << "int winner, weight_sum, sel, selection[" << this->nro <<"];\n\n";
    writer << std::string(indentation, '\t') << "unsigned int universal_mask = (1 << " << this->nro << ") - 1, vertex_array;\n\n";
    writer << std::string(indentation, '\t') << "unsigned char img[" << w*h << "];\n";
    writer << std::string(indentation, '\t') << "int ultimo_x = poligono[0], ultimo_y = poligono[1], dx, dy;\n\n";
    writer << std::string(indentation, '\t') << "std::mt19937 generator;\n\n";

    writer << "void generate_image() {\n";
    indentation++;
    writer << std::string(indentation, '\t') << "std::memset(img, 0," << w*h << "* sizeof(unsigned char));\n\n";

    INSTRUCOES();
    
    writer << std::string(indentation, '\t') << "}\n\n";

    writer << "int main(int argc, char *argv[]){\n\n";
    writer << "\tgenerate_image();\n\n\treturn 0;\n}\n";


    writer.close();

    return true;
}



bool Gerador::CONFIGURACOES(){
    
    match(TipoToken::CONFIGURACOES); //CONFIGURACOES
    match(TipoToken::DEL_ACHA); //{
    
    while(lookahead(0)->tipo != TipoToken::DEL_FCHA){
        switch(lookahead(0)->tipo){
            case TipoToken::POLIGONO:
                if(!POLIGONO()){
                    return false;
                }
            break;
            case TipoToken::ITERACOES:
                if(!ITERACOES()){
                    return false;
                }
            break;
            case TipoToken::TELA:
                if(!TELA()){
                    return false;
                }
            break;
            case TipoToken::SALTO:
                if(!SALTO()){
                    return false;
                }
            break;
        }
    }
    writer << "\n";
    

    writer << "int poligono[" << 3*this->nro << "] = {";
    for(int i = 0; i < this->nro; i++){
        writer << -this->origem_x + this->poligono_aux[3*i+0] << ", ";
        writer << +this->origem_y + this->h - 1 - this->poligono_aux[3*i+1] << ", ";
        writer << this->poligono_aux[3*i + 2];
        if(i < this->nro - 1){
            writer << ", ";
        }
    }
    writer<< "};\n";

    match(TipoToken::DEL_FCHA);
    return true;
}

bool Gerador::POLIGONO(){
    match(TipoToken::POLIGONO); //POLIGONO
    match(TipoToken::DEL_ACHA); //{
    if(lookahead(0)->tipo == TipoToken::MANUAL){
        match(TipoToken::MANUAL);
        int i = 0, n = 0;
        while(lookahead(i)->tipo != TipoToken::DEL_FCHA){
            if(lookahead(i)->tipo == TipoToken::VERTICE){
                n++;
            }
            i++;
        }
        this->nro = n;
        
    

    
        for(i = 0; i < n; i++){
            match(TipoToken::VERTICE);
            this->poligono_aux.push_back(std::stoi(lookahead(0)->lexema));
            match(TipoToken::INTEIRO);
            this->poligono_aux.push_back(std::stoi(lookahead(0)->lexema));
            match(TipoToken::INTEIRO);
            if(lookahead(0)->tipo == TipoToken::INTEIRO){
                this->poligono_aux.push_back(std::stoi(lookahead(0)->lexema));
                match(TipoToken::INTEIRO); 
            }else{
                this->poligono_aux.push_back(1);
            }
        }

        match(TipoToken::DEL_FCHA);

    }else if(lookahead(0)->tipo == TipoToken::INSCRITO){
        match(TipoToken::INSCRITO); //INSCRITO
        match(TipoToken::CENTRO);; //CENTRO
        int x_centro = std::stoi(lookahead(0)->lexema);
        match(TipoToken::INTEIRO); 
        int y_centro = std::stoi(lookahead(0)->lexema);
        match(TipoToken::INTEIRO);
        match(TipoToken::RAIO); //RAIO
        int raio = std::stoi(lookahead(0)->lexema);
        if(raio <= 0){
            error =  "ERRO: O comprimento do raio deve ser natural. Raio utilizado: " + std::to_string(raio);
            return false;
        }
        match(TipoToken::INTEIRO);
        match(TipoToken::NRO);
        int n = std::abs(std::stoi(lookahead(0)->lexema));
        if(n <= 0){
            error =  "ERRO: O numero de vertices deve ser natural. Numero utilizado: " + std::to_string(n);
            return false;
        }
        match(TipoToken::INTEIRO);

        
        double pi = 3.141592;
        for(int i = 0; i < n;i++){
            this->poligono_aux.push_back(x_centro + int(raio*std::cos(i*2*pi/n)));
            this->poligono_aux.push_back(y_centro + int(raio*std::sin(i*2*pi/n)));
            this->poligono_aux.push_back(1);
        }
        this->nro = n;


        match(TipoToken::DEL_FCHA);
    }
    
    return true;
}

bool Gerador::ITERACOES(){
    match(TipoToken::ITERACOES);
    iter = std::stoi(lookahead(0)->lexema);
    if(iter <= 0){
         error = "ERRO: O numero de iteracoes deve ser natural. Numero utilizado: " + std::to_string(iter);
         return false;
    }

    writer << std::string(indentation, '\t') << "int iter = 0, iteracoes = " << iter << ";\n";
    match(TipoToken::INTEIRO);
    return true;
}

bool Gerador::TELA(){
    match(TipoToken::TELA);
    match(TipoToken::DEL_ACHA);
    match(TipoToken::ORIGEM);
    this->origem_x = std::stoi(lookahead(0)->lexema);
    match(TipoToken::INTEIRO);
    this->origem_y = std::stoi(lookahead(0)->lexema);
    match(TipoToken::INTEIRO);
    match(TipoToken::RESOLUCAO);
    w = std::stoi(lookahead(0)->lexema);
    match(TipoToken::INTEIRO);
    h = std::stoi(lookahead(0)->lexema);
    if(w <= 0 || h <= 0){
        error =  "ERRO: Dimensoes da resolucao devem ser naturais. Dimensoes utilizadas: " + std::to_string(w) + " " + std::to_string(h);
        return false;
    }
    match(TipoToken::INTEIRO);
    match(TipoToken::DEL_FCHA);
    return true;
}

bool Gerador::SALTO(){
    match(TipoToken::SALTO);
    if(lookahead(0)->tipo == TipoToken::INTEIRO){
        writer << std::string(indentation, '\t') << "int r_num = " << std::abs(std::stoi(lookahead(0)->lexema));

        match(TipoToken::INTEIRO);
        writer << ", r_dem = " << std::abs(std::stoi(lookahead(0)->lexema));
        match(TipoToken::INTEIRO);
    }else if(lookahead(0)->tipo == TipoToken::REAL){
        double number = std::abs(std::stod(lookahead(0)->lexema)), frac;
        match(TipoToken::REAL);
        frac = number - std::floor(number);
        
        int precisao = 1000000, mdc, num, dem;
        mdc = std::gcd(int(std::round(frac*precisao)), precisao);

        dem = int(precisao/mdc);
        num = int(std::round(frac*precisao)/mdc) + int(number)*dem;
    

        writer << std::string(indentation, '\t') << "int r_num = " << num;
        writer << ", r_dem = " << dem;
    }

    writer << ";\n";

    return true;
}

bool Gerador::INSTRUCOES(){
    match(TipoToken::INSTRUCOES);
    match(TipoToken::DEL_ACHA);

    writer << std::string(indentation, '\t') << "while(iter < iteracoes){\n\n";
    indentation++;
    writer << std::string(indentation, '\t') << "vertex_array = 0;\n";
    writer << std::string(indentation, '\t') << "weight_sum = 0;\n";
    writer << std::string(indentation, '\t') << "sel = 0;\n\n\n";

    COMMANDOS_INSTRUCOES();

    match(TipoToken::DEL_FCHA);
    writer << std::string(indentation, '\t') << "\n\n\n";
    writer << std::string(indentation, '\t') << "for(int i = 0; i < " << this->nro << "; i++){\n";
    indentation++;
    writer << std::string(indentation, '\t') << "if(vertex_array & 1){\n";
    indentation++;
    writer << std::string(indentation, '\t') <<"selection[sel] = i;\n";
    writer << std::string(indentation, '\t') <<"sel++;\n";
    writer << std::string(indentation, '\t') <<"weight_sum+=poligono[3*i+2];\n";

    //if
    indentation--;
    writer << std::string(indentation, '\t') <<"}\n\n";
    writer << std::string(indentation, '\t') <<"vertex_array >>= 1;\n";
    //for
    indentation--;
    writer << std::string(indentation, '\t') <<"}\n\n";

    writer << std::string(indentation, '\t') << "std::uniform_int_distribution<int> distribution(1, weight_sum);\n";
    writer << std::string(indentation, '\t') << "winner = distribution(generator);\n";
    writer << std::string(indentation, '\t') << "for(int i = 0; i < sel; i++){\n";
    indentation++; //for
    
    writer << std::string(indentation, '\t') << "if(poligono[3*selection[i] + 2] >= winner){\n";
    indentation++; //if
    writer << std::string(indentation, '\t') << "winner = selection[i];\n";
    writer << std::string(indentation, '\t') << "break;\n";
    indentation--; //if
    writer << std::string(indentation, '\t') <<"} else{\n";
    indentation++; //else
    
    writer << std::string(indentation, '\t') <<"winner = winner - poligono[3*selection[i] + 2];\n";
    indentation--; //else
    writer << std::string(indentation, '\t') <<"}\n";
    indentation--; //for
    writer << std::string(indentation, '\t') <<"}\n\n";

    if(anterior){
    writer << std::string(indentation, '\t') << "ANTERIOR[head] = winner;\n";
    writer << std::string(indentation, '\t') << "head = (head + 1)\%asize;\n\n";
    }

    writer << std::string(indentation, '\t') << "dx = poligono[3*winner + 0] - ultimo_x;\n";
    writer << std::string(indentation, '\t') << "dy = poligono[3*winner + 1] - ultimo_y;\n";
    writer << std::string(indentation, '\t') <<    "ultimo_x = ultimo_x + (dx*r_num)/r_dem;\n";
    writer << std::string(indentation, '\t') <<    "ultimo_y = ultimo_y + (dy*r_num)/r_dem;\n\n";

    writer << std::string(indentation, '\t') << "if(ultimo_x >= 0 && ultimo_x < " << this->w <<" && ultimo_y >= 0 &&ultimo_y < " << this->h <<")\n";
    indentation++;
    writer << std::string(indentation, '\t') << "img[ultimo_y*" << this->w << " + ultimo_x] = 255;\n";
    indentation--;

    writer << std::string(indentation, '\t') << "iter++;\n";
    //while
    indentation--; 
    writer << std::string(indentation, '\t') <<"}\n\n";

    writer << std::string(indentation, '\t') << "stbi_write_png(\"code.png\", " << this->w << ", " << this->h << ", 1, img, " << this->w << ");\n";

    return true;
}

bool Gerador::COMMANDOS_INSTRUCOES(){

    while(lookahead(0)->tipo != TipoToken::DEL_FCHA){
        switch(lookahead(0)->tipo){
            case TipoToken::INCLUIR:
            case TipoToken::EXCLUIR:
                INCLUIR_EXCLUIR();
            break;
            case TipoToken::IF:
                IF(); 
            break;
        }
    }
    return true;
}

bool Gerador::INCLUIR_EXCLUIR(){
    writer << std::string(indentation, '\t') << "vertex_array";

    TipoToken opcao_tipo_op = lookahead(0)->tipo;
    switch(lookahead(0)->tipo){
        case TipoToken::INCLUIR:
            opcao_tipo_op = TipoToken::INCLUIR;
            match(TipoToken::INCLUIR);
            writer << " |= ";
        break;
        case TipoToken::EXCLUIR: 
            opcao_tipo_op = TipoToken::EXCLUIR;
            match(TipoToken::EXCLUIR);
            writer << " &= ~";        
        break;
    }

    if(lookahead(0)->tipo == TipoToken::TODOS){
        match(TipoToken::TODOS);
        writer << "universal_mask;\n";
        return true; 
    }

    //VERTICE || ANTERIOR
    TipoToken opcao_tipo_vertice = lookahead(0)->tipo;
    match(opcao_tipo_vertice);

    match(TipoToken::DEL_APAR);
    int indice = EXPR_INT_1();
    match(TipoToken::DEL_FPAR);

    // + 
    int offset = 0;
    if((lookahead(0)->tipo == TipoToken::OP_ADD || lookahead(0)->tipo == TipoToken::OP_SUB) &&
       (lookahead(1)->tipo == TipoToken::INTEIRO)){
        TipoToken opcao_tipo_op_offset = lookahead(0)->tipo;
        match(opcao_tipo_op_offset);
        offset = EXPR_INT_1()%this->nro;
        if(opcao_tipo_op_offset == TipoToken::OP_SUB){
            offset *= -1;
        }

        if(offset < 0){
            offset = this->nro + offset;
        }
    }

    // + []
    unsigned int array = 0;
    if((lookahead(0)->tipo == TipoToken::OP_ADD || lookahead(0)->tipo == TipoToken::OP_SUB) &&
       (lookahead(1)->tipo == TipoToken::DEL_ACOL)){
        TipoToken opcao_tipo_op_offset = lookahead(0)->tipo;
        match(opcao_tipo_op_offset);
        match(TipoToken::DEL_ACOL);

        while(lookahead(0)->tipo != TipoToken::DEL_FCOL){
            int  eval = EXPR_INT_1()%this->nro;
            if(opcao_tipo_op_offset == TipoToken::OP_SUB){
                eval *= -1;
            }

            if(eval < 0){
                eval = this->nro + eval;
            }

            array |= (1 << eval);

            if(lookahead(0)->tipo == TipoToken::DEL_COM){
                match(TipoToken::DEL_COM);
            }
        }

        match(TipoToken::DEL_FCOL);
    }else{
        array = 1;
    }


    writer << "L_CSHIFT(" << std::to_string(array) <<", ";

    switch(opcao_tipo_vertice){
        case TipoToken::ANTERIOR:
            if(offset != 0){
                writer << "(";
            }
            writer << "ANTERIOR[(head - " << indice << " + asize)\%asize]";
            if(offset != 0){
                writer << " + " << std::to_string(offset) + ")\%" + std::to_string(this->nro);
            }
            //buffer[(head - (position) + bsize)%bsize];
        break;
        case TipoToken::VERTICE: 
            writer << (indice + offset)%this->nro;
        break;
    }

    writer << " ,"  << this->nro << ", universal_mask);\n";
    //L_CSHIFT(1 << getbuffer(1), 1, vsize, mask_universal)

    return true;
}

bool Gerador::IF(){
    match(TipoToken::IF);
    match(TipoToken::DEL_APAR);
    Token* T;
    writer << std::string(indentation, '\t') <<"if(";

    TipoToken opcao_tipo_vertice;
    int indice, offset;
    while(lookahead(0)->tipo != TipoToken::DEL_FPAR ||
          lookahead(1)->tipo != TipoToken::DEL_ACHA){
        T = lookahead(0);
        switch(T->tipo){
            case TipoToken::VERTICE:
            case TipoToken::ANTERIOR:
                opcao_tipo_vertice = T->tipo;
                match(opcao_tipo_vertice);

                match(TipoToken::DEL_APAR);
                indice = EXPR_INT_1();
                match(TipoToken::DEL_FPAR);

                offset = 0;
                if((lookahead(0)->tipo == TipoToken::OP_ADD || lookahead(0)->tipo == TipoToken::OP_SUB) &&
                    (lookahead(1)->tipo == TipoToken::INTEIRO)){
                    
                    TipoToken opcao_tipo_op_offset = lookahead(0)->tipo;
                    match(opcao_tipo_op_offset);
                    offset = EXPR_INT_1()%this->nro;
                    if(opcao_tipo_op_offset == TipoToken::OP_SUB){
                        offset *= -1;
                    }

                    if(offset < 0){
                        offset = this->nro + offset;
                    }
                }

                switch(opcao_tipo_vertice){
                    case TipoToken::VERTICE:
                        writer << (indice + offset)%this->nro;
                    break;
                    case TipoToken::ANTERIOR:
                        if(offset != 0){
                            writer << "(";
                        }
                        writer << "ANTERIOR[(head - " << indice << " + asize)\%asize]";
                        if(offset != 0){
                            writer << " + " << std::to_string(offset) + ")\%" + std::to_string(this->nro);
                        }
                    break;
                }
                

            break;
            default:
                writer << T->lexema << " ";
                match(T->tipo);
            break;
        }
    }
    match(TipoToken::DEL_FPAR);
    match(TipoToken::DEL_ACHA);
    writer << "){\n";
    this->indentation++;

    COMMANDOS_INSTRUCOES();

    this->indentation--;
    match(TipoToken::DEL_FCHA);
    writer << std::string(indentation, '\t') << "}";
    //PARE AQUI
    if(lookahead(0)->tipo == TipoToken::ELSE){
        match(TipoToken::ELSE);
        writer << " else ";
        switch(lookahead(0)->tipo){
            case TipoToken::DEL_ACHA:
                match(TipoToken::DEL_ACHA);
                writer << "{\n";
                indentation++;
                COMMANDOS_INSTRUCOES();
                indentation--;
                match(TipoToken::DEL_FCHA);
                writer << std::string(indentation, '\t')  <<"}";
            break;
            case TipoToken::IF:
                writer << "\n";
                IF();
            break;
        }
    }
    writer << "\n";

    return true;
}

int Gerador::EXPR_INT_1(){

    int result;
    result = EXPR_INT_2();

    if(lookahead(1)->tipo == TipoToken::INTEIRO || lookahead(1)->tipo == TipoToken::DEL_APAR)
        result = EXPR_INT_1_AUX(result);

    return result;
}

int Gerador::EXPR_INT_1_AUX(int lhs){
    int result;
    if(lookahead(0)->tipo == TipoToken::OP_ADD || lookahead(0)->tipo == TipoToken::OP_SUB){
        switch(lookahead(0)->tipo){
            case TipoToken::OP_ADD:
                match(TipoToken::OP_ADD);
                result = lhs + EXPR_INT_2();
            break;
            case TipoToken::OP_SUB:
                match(TipoToken::OP_SUB);
                result = lhs - EXPR_INT_2();
            break;
        }
        return EXPR_INT_1_AUX(result);
    }

    return lhs;
    
}


int Gerador::EXPR_INT_2(){
    
    int result;
    result =EXPR_INT_3();

    if(lookahead(1)->tipo == TipoToken::INTEIRO || lookahead(1)->tipo == TipoToken::DEL_APAR)
        result = EXPR_INT_2_AUX(result);

    return result;    
}

int Gerador::EXPR_INT_2_AUX(int lhs){
    int result;
    if(lookahead(0)->tipo == TipoToken::OP_MUL || lookahead(0)->tipo == TipoToken::OP_DIV){
        switch(lookahead(0)->tipo){
            case TipoToken::OP_MUL:
                match(TipoToken::OP_MUL);
                result = lhs * EXPR_INT_3();
            break;
            case TipoToken::OP_DIV:
                match(TipoToken::OP_DIV);
                result = lhs / EXPR_INT_3();
            break;
        }
        

        return EXPR_INT_2_AUX(result);
    }

    return lhs;
}



int Gerador::EXPR_INT_3(){
    int result;
    switch(lookahead(0)->tipo){
        case TipoToken::DEL_APAR:
            match(TipoToken::DEL_APAR);
                
            result = EXPR_INT_1();

            match(TipoToken::DEL_FPAR);

            return result;
        break;
        case TipoToken::INTEIRO:
            result = std::stoi(lookahead(0)->lexema);
            match(TipoToken::INTEIRO);
            return result;
        break;
        default:
            return false;
    }
}