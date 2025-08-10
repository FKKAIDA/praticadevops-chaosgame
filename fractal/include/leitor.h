#ifndef LEITOR_H
#define LEITOR_H
#include <iostream>
#include <string>


class Leitor {
    public:
        Leitor(std::string path);
        ~Leitor();
        int readnext();
        void dec();
        std::string getlexama();
        void reiniciar();
        void confirmar();
        int start, end, size;
    private:

        std::string input;
};

#endif