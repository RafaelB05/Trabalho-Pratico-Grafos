#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

using namespace std;

struct Instancia{
    int tamanho_instancia;
    int capacidade_veiculo;
    int roterizacao;
    int janela_de_tempo;
};

void LerInstancia(string namefile, Instancia &instancia){
    char delimitador = ' ';
    ifstream entrada(namefile);
    string campoAtual;
    for(int i = 1; i <= 10; i++){
        getline(entrada,campoAtual,delimitador);
        getline(entrada,campoAtual);
        switch (i)
        {
        case 5:
            instancia.tamanho_instancia = stoi(campoAtual);
            break;

        case 8:
            instancia.roterizacao = stoi(campoAtual);
            break;
        case 9:
            instancia.janela_de_tempo = stoi(campoAtual);
            break;
        case 10:
            instancia.capacidade_veiculo = stoi(campoAtual);
            break;
        default:
            break;
        }
    }
}

int main(){

    string nomeArquivo;

    cin >> nomeArquivo;

    Instancia instancia;

    LerInstancia(nomeArquivo,instancia);


    return 0;
}