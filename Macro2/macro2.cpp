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
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>
#include "funcoes.h"

using namespace std;

int ref = 1;

vector<int> geraRotas(Instancia inst, Pontos *totalPontos, int index, Truck caminhao){
    vector<int> rota;
    rota.push_back(0);
    rota.push_back(index);
    rota.push_back(totalPontos[index].pEntrega);
    rota.push_back(0);
    ref++;
    for (int i = 0; i < int(rota.size()); i++){
        caminhao.tempoGasto += totalPontos[rota[i]].duracaoServico;
        caminhao.ocupacao += totalPontos[rota[i]].demanda;
    }
    return rota;
}

int main(){
    cout << "BEM VINDO AO GERADOR DE ROTAS" << endl;

    string namefile;
	
    cout << "Insira a instancia a qual deseja testar: ";
    cin >> namefile;
	
	
    Instancia instancia;

    LerInstancia(namefile,instancia);

    vector<Truck> caminhoes;

    Pontos *totalPontos = new Pontos[instancia.tamanho_instancia];
    int **MA = new int*[instancia.tamanho_instancia];

    leituraGrafo(namefile, instancia.tamanho_instancia, MA, totalPontos);

    Pontos *coletas = new Pontos[(instancia.tamanho_instancia - 1 ) / 2];

    vector<int> rota;
    int finish = 0;
    
    while (finish != 1){
        Truck caminhao;
        int index = coletas[0 + ref].id;

        label1:
        if (!coletas[0 + ref].visitado)
            rota = geraRotas(instancia, totalPontos, index, caminhao);
        else{
            ref++;
            goto label1;
        }
        caminhao.rota = mais_proximo(caminhao, rota, MA, instancia, totalPontos, finish);
        caminhoes.push_back(caminhao);
    }
    

    cout << "Total de caminhoes: " << caminhoes.size() << endl;
    int custo = 0;
    for (int i = 0; i < int(caminhoes.size()); i++){
        custo += custo_total(caminhoes[i].rota, MA);
    }
    cout << "Custo total: " << custo << endl;
};
