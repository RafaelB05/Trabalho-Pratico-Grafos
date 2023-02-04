/*
MacroEntrega 1: Leitura de Dados, Estruturas e verificação de restrição
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
    int capacity = 0;
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

void randomNumber(int *vetor, int nodesNumber, Instancia inst){
    
    srand(time(0)); // Seta a raiz da geração de numeros como o tempo atual

    for (int i = 0; i < nodesNumber; i++) {
        int random_number = (rand() % ((inst.tamanho_instancia) / 2) - 1) + 1; // Gera X numeros aleatorios baseado no tamanho da instancia
        vetor[i] = random_number;
    }

}

bool verificaRestricao(Pontos *vetor, Instancia inst, int **MA){

    bool worth = true;
    Pontos Deposito = vetor[0];

    Truck caminhao;

    //Deposito -> 5 -> 12 -> 15 -> 22 -> Deposito #6
    //Demanda: 0 + 30 + 24 + (-30) + (-24) + 0
    //Tempo: (0)0 + [0, 5]15 + (5)3 + [5, 12]16 + (12)4 + [12, 15]20 + (15)5 + [15, 22]10 + (22)5 + [22, 0]19

    /*-------SETANDO SEQUENCIA DE NODES E SEQUENCIA DE NODES POSTERIOR-------*/

    int nodesNumber;

    cout << "Insira a quantidade de nodes que deseja testar: ";
    cin >> nodesNumber;

    int sequenciaNodes[nodesNumber];
    int sequenciaNodesPosterior[nodesNumber];
    
    randomNumber(sequenciaNodes, nodesNumber, inst);
    
    sequenciaNodes[0] = 0; // Seta inicio do trajeto como Deposito (0)
    sequenciaNodes[nodesNumber - 1] = 0; // Seta fim do trajeto como Deposito (0)
    
    cout << "Os Nodes selecionados foram: ";
    for (int i = 0; i < nodesNumber; i++) 
        cout << sequenciaNodes[i] << " ";

    cout << endl;

    for (int i = 0; i < nodesNumber; i++) 
        sequenciaNodesPosterior[i] = vetor[sequenciaNodes[i]].pEntrega;

    /*-------SETADO SEQUENCIA DE NODES E SEQUENCIA DE NODES POSTERIOR-------*/

    for (int i = 0; i < nodesNumber; i++){ //ENCHENDO O CAMINHÃO
        caminhao.capacity += vetor[sequenciaNodes[i]].demanda;

        if (caminhao.capacity > inst.capacidade_veiculo)
        {
            worth = false;
            cout << "!!!Capacidade Maxima atingida!!!" << endl;
            break;
        }
    }

    cout << "Capacidade usada total de: ";
    cout << caminhao.capacity << endl;

    //ESVAZIANDO CAMINHÃO
    for (int i = 0; i < nodesNumber; i++){
        caminhao.capacity -= vetor[sequenciaNodesPosterior[i]].demanda;
    }

    for (int i = 0; i < nodesNumber; i++){
        caminhao.tempoGasto += MA[sequenciaNodes[i]][sequenciaNodesPosterior[i]];
        caminhao.tempoGasto += vetor[sequenciaNodes[i]].duracaoServico;
        caminhao.tempoGasto += vetor[sequenciaNodesPosterior[i]].duracaoServico;
        if (caminhao.tempoGasto > inst.roterizacao)
        {
            cout << "!!!Tempo Maximo Exedido!!!" << endl;
            break;
        }
    }

    cout << "Tempo gasto total de: ";
    cout << caminhao.tempoGasto << endl;

    return worth;

}


int main(){

    string posicaoAtualArquivo;

    cin >> posicaoAtualArquivo;

    Instancia instancia;

    LerInstancia(posicaoAtualArquivo,instancia);

    Pontos *totalPontos = new Pontos[instancia.tamanho_instancia];
    int **MA = new int*[instancia.tamanho_instancia];


    leituraGrafo(posicaoAtualArquivo,instancia.tamanho_instancia,MA,totalPontos);
    
    if (verificaRestricao(totalPontos, instancia, MA) == 1)
        cout << "Verificado com sucesso!" << endl;
    else
        cout << "Verificado com falhas..." << endl;


    return 0;
}