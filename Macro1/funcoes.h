#ifndef funcoes
#define funcoes

#include <iostream>
#include <fstream>
#include <cstring>
#include <time.h>
#include <unistd.h>
#include <cstdlib>
#include <vector>

using namespace std;

struct Instancia{
    int tamanho_instancia;
    int capacidade_veiculo;
    int roterizacao;
    int janela_de_tempo;
};

struct Pontos{
    unsigned int id;
    float latitude;
    float longitude;
    int demanda;
    int menorTempo;
    int tempoLimite;
    int duracaoServico;
    int pColeta;
    int pEntrega;
};

struct Truck{
    unsigned int id;
    int tempoGasto = 0;
    int ocupacao = 0;
     vector <Pontos> sequencia;
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
    entrada.close();
}

void leituraGrafo(string nameFile,int tamanhoGrafo,int **MA,Pontos *totalPontos){
    char delimitador = ' ';
    ifstream entrada(nameFile);
    string posicaoAtual;

    for(int i = 0; i < tamanhoGrafo; i++){
        MA[i] = new int[tamanhoGrafo];
    }

    for(int i = 0; i < 10;i++) // Descarta as 10 primeiras linhas que ja foram lidas pois caracterizam a instancia
        getline(entrada,posicaoAtual);

    getline(entrada,posicaoAtual);
    for(int i = 0; i < tamanhoGrafo;i++){
        for(int j = 1; j <= 9; j++ ){
            getline(entrada,posicaoAtual,delimitador);
            switch (j){
                case 1:
                    totalPontos[i].id = stoi(posicaoAtual);
                    break;
                
                case 2:
                    totalPontos[i].latitude = stof(posicaoAtual);
                    break;
                case 3:
                    totalPontos[i].longitude = stof(posicaoAtual);
                    break;

                case 4:
                    totalPontos[i].demanda = stoi(posicaoAtual);
                    break;
                
                case 5:
                    totalPontos[i].menorTempo = stoi(posicaoAtual);
                    break;
                
                case 6:
                    totalPontos[i].tempoLimite =  stoi(posicaoAtual);
                    break;
                
                case 7:
                    totalPontos[i].duracaoServico =  stoi(posicaoAtual);
                    break;
                
                case 8:
                    totalPontos[i].pColeta = stoi(posicaoAtual);
                    delimitador = '\n';
                    break;
                
                case 9:
                    totalPontos[i].pEntrega = stoi(posicaoAtual);
                    delimitador = ' ';
                    break;
                
                default:
                    break;
            }
        }
    }

    getline(entrada,posicaoAtual);
    for (int i = 0; i < tamanhoGrafo; i++){
        delimitador = ' ';
        for(int j = 0; j < tamanhoGrafo;j++){
            getline(entrada,posicaoAtual,delimitador);
            MA[i][j] = stoi(posicaoAtual);
            if(j == tamanhoGrafo-2)
                delimitador = '\n';
        }

    }
}


bool verificaRestricao(Pontos *totalPontos, Instancia inst,Truck caminhao){
    
    int visitaEntrega, visitaColeta;
    Pontos coleta;
    Pontos entrega;
    bool restrito = false;

    //precedencia de coleta
    for(int i = 0; i < int(caminhao.sequencia.size()); i++){
        restrito = false;
        if(caminhao.sequencia[i].pColeta > 0){
            for(int j = 0; j < i ;j++){
                if(caminhao.sequencia[j].id == caminhao.sequencia[i].pColeta)
                    restrito = true;
            }
            if(restrito == false)
                return false;
            
        }
    }

    //Janela de Tempo
    for(int i = 0; i < int(caminhao.sequencia.size()); i++){
        visitaEntrega = visitaColeta = 0;
        if(caminhao.sequencia[i].pColeta == 0 and caminhao.sequencia[i].id != 0){
            entrega = totalPontos[caminhao.sequencia[i].pEntrega];
        }
        else if(caminhao.sequencia[i].pEntrega == 0 and caminhao.sequencia[i].id != 0)
            entrega = caminhao.sequencia[i];
        
        if(caminhao.tempoGasto > entrega.tempoLimite)
            return false;

            
    }
    // Obrigatoriedade e Exclusividade de visita
    for(int i = 0; i < int(caminhao.sequencia.size()); i++){
        visitaEntrega = 0;
        visitaColeta = 0;
        if(caminhao.sequencia[i].pColeta == 0 and caminhao.sequencia[i].id != 0){
            coleta = caminhao.sequencia[i];
            entrega = totalPontos[caminhao.sequencia[i].pEntrega-1];
        }
        else if(caminhao.sequencia[i].pEntrega == 0 and caminhao.sequencia[i].id != 0){
            entrega = caminhao.sequencia[i];
            coleta = totalPontos[caminhao.sequencia[i].pColeta-1];
        }

        for(int j = 0; j < caminhao.sequencia.size();j++){
            if(caminhao.sequencia[j].id == coleta.id)
                visitaColeta++;

            if(caminhao.sequencia[j].id == entrega.id)
                visitaEntrega++;
        }
        if(visitaColeta > 1 or visitaEntrega > 1)
            return false;


    }

    //Verifica inicio e fim e se não estoura o tempo de roterização
    if(caminhao.sequencia[0].id != 0 or caminhao.sequencia[caminhao.sequencia.size()-1].id != 0 or caminhao.tempoGasto > inst.roterizacao)
        return false;
        
    //Capacidade do veiculo
    if(caminhao.ocupacao > inst.capacidade_veiculo)
        return false;


    return true; // caso passe por todas as restrições retorna verdadeiro
}

#endif