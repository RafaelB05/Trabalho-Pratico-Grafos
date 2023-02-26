#ifndef funcoes
#define funcoes

#include <iostream>
#include <fstream>
#include <cstring>
#include <time.h>
#include <unistd.h>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

const int INF = 1000000;

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
    int tempoAbertura;
    int tempoLimite;
    int duracaoServico;
    int pColeta;
    int pEntrega;
    bool visitado = false;
};

struct Truck{
    unsigned int id;
    int tempoGasto = 0;
    int ocupacao = 0;
     vector <int> rota;
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
                    totalPontos[i].tempoAbertura = stoi(posicaoAtual);
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

bool verificaRestricao(Pontos *totalPontos, Instancia inst, Truck caminhao){
    
    int visitaEntrega, visitaColeta;
    Pontos coleta;
    Pontos entrega;
    Pontos atual;
    bool restrito = false;

    //precedencia de coleta
    for(int i = 0; i < int(caminhao.rota.size()); i++){
        restrito = false;
        if(totalPontos[caminhao.rota[i]].pColeta > 0){
            for(int j = 0; j < i ;j++){
                if(totalPontos[caminhao.rota[j]].id == totalPontos[caminhao.rota[i]].pColeta)
                    restrito = true;
            }
            if(restrito == false)
                return false;
            
        }
    }

    //Verifica se chegou antes do tempo de abertura ou se estourou o tempo de fechamento
    for(int i = 1; i < int(caminhao.rota.size()); i++){
        atual = totalPontos[caminhao.rota[i]];
        if(caminhao.tempoGasto < atual.tempoAbertura){
            caminhao.tempoGasto += atual.tempoAbertura - caminhao.tempoGasto;
        }
        else if(caminhao.tempoGasto > atual.tempoLimite){
            return false;
        }
    }

    // Obrigatoriedade e Exclusividade de visita
    for(int i = 0; i < int(caminhao.rota.size()); i++){
        visitaEntrega = 0;
        visitaColeta = 0;
        if(totalPontos[caminhao.rota[i]].pColeta == 0 and totalPontos[caminhao.rota[i]].id != 0){
            coleta = totalPontos[caminhao.rota[i]];
            entrega = totalPontos[totalPontos[caminhao.rota[i]].pEntrega-1];
        }
        else if(totalPontos[caminhao.rota[i]].pEntrega == 0 and totalPontos[caminhao.rota[i]].id != 0){
            entrega = totalPontos[caminhao.rota[i]];
            coleta = totalPontos[totalPontos[caminhao.rota[i]].pColeta-1];
        }

        for(int j = 0; j < int(caminhao.rota.size()); j++){
            if(totalPontos[caminhao.rota[j]].id == coleta.id)
                visitaColeta++;

            if(totalPontos[caminhao.rota[j]].id == entrega.id)
                visitaEntrega++;
        }
        if(visitaColeta > 1 or visitaEntrega > 1)
            return false;
    }

    // Verifica inicio e fim e se não estoura o tempo de roterização
    if(caminhao.rota[0] != 0 or caminhao.rota[caminhao.rota.size()-1] != 0 or caminhao.tempoGasto > inst.roterizacao){
        return false;
    }
        
    //Capacidade do veiculo
    if(caminhao.ocupacao > inst.capacidade_veiculo){
        return false;
    }
    return true; // caso passe por todas as restrições retorna verdadeiro
}

vector<pair<int, int>> pega_pares(vector<int> lista){
    vector<pair<int, int>> pares;
    for (int i = 0; i < int(lista.size()) - 1; i++) {
        pares.push_back(make_pair(lista[i], lista[i + 1]));
    }
    pares.push_back(make_pair(lista[lista.size() - 1], lista[0]));
    
    return pares;
}

int custo_total(const vector<int> &ciclo, int** MA) {
    int custo = 0;
    vector<pair<int, int>>pares = pega_pares(ciclo);
    for (int i = 0; i < int(pares.size()) - 1; i++)
    {
        int a = pares[i].first;
        int b = pares[i].second;
        custo = MA[a][b] + custo;
        //cout << "custo = +" << MA[a][b] << " - " << custo << endl;
    }
    //cout << endl;

    return custo;
}

//Heuristica pra escolha do ponto de inserção
vector<int> mais_proximo(Truck caminhao, vector<int>ciclo_inicial, int** MA, Instancia inst, Pontos *totalPontos, int &finish) {
    while (true) {
        int distancia_vertice = INF;
        int custo_coleta = INF;
        int vertice = -1;

        for (int linha = 0; linha < (inst.tamanho_instancia - 1) / 2; linha++) {
            if (find(ciclo_inicial.begin(), ciclo_inicial.end(), linha) == ciclo_inicial.end()) {
                for (int coluna = 0; coluna < inst.tamanho_instancia; coluna++) {
                    if (find(ciclo_inicial.begin(), ciclo_inicial.end(), coluna) != ciclo_inicial.end()) {
                        if (MA[linha][coluna] < distancia_vertice) {
                            if (!totalPontos[linha].visitado){   
                                distancia_vertice = MA[linha][coluna];
                                vertice = linha;
                            }
                            else
                                linha++;
                        }
                    }
                }
            }
        }

        if (vertice != -1){
            totalPontos[vertice].visitado = true;
            totalPontos[totalPontos[vertice].pEntrega].visitado = true;

            vector<pair<int, int>> pares = pega_pares(ciclo_inicial);
            
            caminhao.ocupacao += totalPontos[vertice].demanda;
            caminhao.tempoGasto += totalPontos[vertice].duracaoServico;
            caminhao.ocupacao += totalPontos[totalPontos[vertice].pEntrega].demanda;
            caminhao.tempoGasto += totalPontos[totalPontos[vertice].pEntrega].duracaoServico;

            for (pair<int, int> aresta : pares) {
                int novo_custo_coleta = MA[vertice][aresta.first] + MA[vertice][aresta.second] - MA[aresta.first][aresta.second];
                if (novo_custo_coleta < custo_coleta) {
                    
                    caminhao.rota = ciclo_inicial;

                    custo_coleta = novo_custo_coleta;
                    
                    auto it = find(caminhao.rota.begin(), caminhao.rota.end(), aresta.first);
                    caminhao.rota.insert((caminhao.rota.begin() + int(it - caminhao.rota.begin() + 1)), vertice);

                    it = find(caminhao.rota.begin(), caminhao.rota.end(), vertice);
                    caminhao.rota.insert((caminhao.rota.begin() + int(it - caminhao.rota.begin() + 1)), totalPontos[vertice].pEntrega);
                    if (verificaRestricao(totalPontos, inst, caminhao)){
                        caminhao.tempoGasto += custo_coleta;
                        continue;
                    }
                    else{
                        caminhao.tempoGasto += custo_coleta;
                        return ciclo_inicial;
                    }
                }
            }
        }
        else{
            finish = 1;
            return ciclo_inicial;
        }

        ciclo_inicial = caminhao.rota;

    }

}

#endif
