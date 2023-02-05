/*
MacroEntrega 1: Leitura de Dados, Estruturas e verificação de restrição
A solução aleatória foi gerada a partir da instancia poa-n100-6
Enzo Velo : 202120910
Lucas Gomes Colombo : 202120795
Rafael Brunini : 202120488
*/
#include <iostream>
#include <fstream>
#include <cstring>
#include <time.h>
#include <unistd.h>
#include <cstdlib>
#include "funcoes.h"

using namespace std;


int main(){

    Truck caminhao;

    string posicaoAtualArquivo;

    cout << "insira a instancia q deseja testar: "; 
    cin >> posicaoAtualArquivo;

    Instancia instancia;

    LerInstancia(posicaoAtualArquivo,instancia);

    Pontos *totalPontos = new Pontos[instancia.tamanho_instancia];
    int **MA = new int*[instancia.tamanho_instancia];

    leituraGrafo(posicaoAtualArquivo,instancia.tamanho_instancia,MA,totalPontos);
    caminhao.sequencia.push_back(totalPontos[0]);
    caminhao.tempoGasto += MA[0][4];
    caminhao.sequencia.push_back(totalPontos[4]); //coleta
    caminhao.ocupacao +=totalPontos[4].demanda;
    caminhao.tempoGasto += MA[4][11] + 5;
    caminhao.sequencia.push_back(totalPontos[11]); // coleta
    caminhao.ocupacao += totalPontos[11].demanda;
    caminhao.tempoGasto += MA[11][61] + 5;
    caminhao.sequencia.push_back(totalPontos[61]); //entrega
    caminhao.ocupacao += totalPontos[61].demanda;
    caminhao.tempoGasto += MA[61][51] + 5;
    caminhao.sequencia.push_back(totalPontos[54]); //entrega
    caminhao.ocupacao += totalPontos[54].demanda;
    caminhao.tempoGasto += MA[54][0] + 5;
    caminhao.sequencia.push_back(totalPontos[0]);

    cout << verificaRestricao(totalPontos,instancia,caminhao);

    return 0;
}