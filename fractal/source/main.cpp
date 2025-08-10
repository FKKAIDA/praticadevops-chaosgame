#include <cmath>
#include <sstream>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <fstream>  
#include "crow_all.h"
#include "leitor.h"
#include "alexico.h"
#include "token.h"
#include "asintatico.h"
#include "gerador.h"

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/fractal").methods("POST"_method)
    ([](const crow::request& req) {
        std::string body = req.body;

        //análise léxica
        ALexico AL(body);
        std::vector<Token*> V = AL.gettokenlist();
        if(V.back()->tipo == TipoToken::ERRO){
            std::string msg = "ERRO LÉXICO. Primeiro char sem match: " + V.back()->lexema + "\nLinha: " + std::to_string(AL.ln_count());
            for(int i = 0; i < V.size();i++){
                delete V[i];
                V[i] = nullptr;
            }
            return crow::response(500, std::string("txt"), msg);
        }
        
        ASintatico AS(V);

        //Análise sintática, começando com a primeira regra de produção
        if(!AS.Start()){
            std::string msg = "ERRO SINTÁTICO. " + AS.error + "\nLinha: " + std::to_string(AS.ln_count());
            for(int i = 0; i < V.size();i++){
                delete V[i];
                V[i] = nullptr;
            }
            return crow::response(500, std::string("txt"), msg);
        }

        Gerador G(V);
        if(!G.gerar()){
            std::string msg = "ERRO SEMÂNTICO/GERADOR. " + G.error + "\nLinha: " + std::to_string(G.ln_count());
            for(int i = 0; i < V.size();i++){
                delete V[i];
                V[i] = nullptr;
            }
            return crow::response(500, std::string("txt"), msg);
        }

        system("g++ -Iinclude code.cpp -o code");
        system("./code");
        
        for(int i = 0; i < V.size();i++){
            delete V[i];
            V[i] = nullptr;
        }

        std::fstream fs("code.png", std::ios::in | std::ios::binary);
        std::ostringstream ss;
        ss<< fs.rdbuf();

        fs.close();
        return crow::response(200, std::string("png"), std::move(ss.str()));
        
    });


    app.port(std::stoi(std::getenv("PORT"))).run();
}
