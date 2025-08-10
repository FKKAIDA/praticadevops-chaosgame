#include <cmath>
#include <sstream>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <fstream>
#include<algorithm> 
#include "crow_all.h"

std::string data(){
    time_t t;
    struct tm * info;
    time (&t);
    info = localtime (&t);
    info->tm_year +=1900;
    info->tm_mon +=1;
    return std::to_string(info->tm_year) + "-" + 
           (info->tm_mon < 10 ? "0" : "") + std::to_string(info->tm_mon) + "-" + 
           (info->tm_mday < 10 ? "0" : "") + std::to_string(info->tm_mday) + "--" + 
           (info->tm_hour < 10 ? "0" : "") + std::to_string(info->tm_hour) + "-" + 
           (info->tm_min < 10 ? "0" : "") + std::to_string(info->tm_min) + "-" + 
           (info->tm_sec < 10 ? "0" : "") + std::to_string(info->tm_sec);
}

std::string d = "";

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/list-examples").methods("GET"_method)
    ([]() {

        std::vector<std::string> L;
        for (std::filesystem::directory_entry const& dir_entry : std::filesystem::directory_iterator("examples")){

            if(dir_entry.path().extension() == ".txt"){
                L.push_back(dir_entry.path().stem());
            }
        }
        std::sort(L.begin(),L.end());
        crow::json::wvalue wval;
        wval["entrylist"] = L;        

        return crow::response(wval);        
    });
    
    CROW_ROUTE(app, "/load-example").methods("POST"_method)
    ([](const crow::request& req) {
        std::string body = req.body;

        std::fstream fs("examples/" + body + ".txt", std::ios::in | std::ios::binary);
        std::ostringstream ss;
        ss << fs.rdbuf();
        fs.close();
        return crow::response(ss.str());
    });

    CROW_ROUTE(app, "/list-history").methods("GET"_method)
    ([]() {

        std::vector<std::string> L;
        for (std::filesystem::directory_entry const& dir_entry : std::filesystem::directory_iterator("history")){
            if(dir_entry.path().extension() == ".txt"){
                L.push_back(dir_entry.path().stem());
            }
        }
        std::sort(L.begin(),L.end());
        crow::json::wvalue wval;
        wval["entrylist"] = L;        

        return crow::response(wval);        
    });

    CROW_ROUTE(app, "/add-text-history").methods("POST"_method)
    ([](const crow::request& req) {
        std::string body = req.body;

        d = data();

        std::ofstream ofs("history/" + d + ".txt");
        ofs << body;

        ofs.close();
        return crow::response(d);
    });

    CROW_ROUTE(app, "/add-image-history").methods("POST"_method)
    ([](const crow::request& req) {
        std::string body = req.body;


        std::ofstream ofs("history/" + d + ".bin", std::ios::binary);
        ofs << body;

        ofs.close();
        return crow::response(d);
    });

    CROW_ROUTE(app, "/load-text-history").methods("POST"_method)
    ([](const crow::request& req) {
        std::string body = req.body;

        std::fstream fs("history/" + body + ".txt", std::ios::in | std::ios::binary);
        std::ostringstream ss;
        ss << fs.rdbuf();
        fs.close();
        return crow::response(ss.str());
    });

    CROW_ROUTE(app, "/load-image-history").methods("POST"_method)
    ([](const crow::request& req) {
        std::string body = req.body;

        std::fstream fs("history/" + body + ".bin", std::ios::in | std::ios::binary);
        std::ostringstream ss;
        ss << fs.rdbuf();
        fs.close();
        return crow::response(200, std::string("bin"), std::move(ss.str()));
    });

    app.port(std::stoi(std::getenv("PORT"))).run();
}
