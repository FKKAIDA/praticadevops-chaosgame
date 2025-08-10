#include "leitor.h"

Leitor::Leitor(std::string s){
  //input = s.copy.c_str();
  input = s;
  size = s.length() + 1;
  start = 0;
  end = 0;
}


int Leitor::readnext(){
    //fim do vetor/"EOF"
    if(end == size){
        return -1;
    }else{
        return input[end++];
    }
}

void Leitor::dec(){
    end--;
}

std::string Leitor::getlexama(){
    std::string s = "";

    int i = start;
    do{
        s += input[i];
        i++;
    }while(i != end);

    return s;

}

void Leitor::confirmar(){
    start = end;
}

void Leitor::reiniciar(){
    end = start;
}

Leitor::~Leitor(){
}