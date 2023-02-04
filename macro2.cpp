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
int main(){

    string posicaoAtualArquivo;

    cin >> posicaoAtualArquivo;

    Instancia instancia;

    LerInstancia(posicaoAtualArquivo,instancia);

    Pontos *totalPontos = new Pontos[instancia.tamanho_instancia];
    int **MA = new int*[instancia.tamanho_instancia];


    leituraGrafo(posicaoAtualArquivo,instancia.tamanho_instancia,MA,totalPontos);

    cout << instancia.tamanho_instancia << ' ' << instancia.roterizacao << ' ' <<  instancia.janela_de_tempo<< ' ' << instancia.capacidade_veiculo << endl;

    for(int i = 0; i < instancia.tamanho_instancia;i++){
         cout << totalPontos[i].id << endl;
    }
    

    for (int i = 0; i < instancia.tamanho_instancia; i++){
        for(int j = 0; j < instancia.tamanho_instancia;j++)
            cout << MA[i][j] << " ";
        cout << endl;
    }
    return 0;
}