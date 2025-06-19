#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include "Simulador.hpp"
#include "Estruturas.hpp"
int main(int argc, char* argv[]) {
    if (argc != 2) {
        //std::cerr << "ERRO: Uso incorreto." << std::endl;
        //std::cerr << "Formato esperado: " << argv[0] << " <caminho_para_o_arquivo.wkl>" << std::endl;
        return 1;
    }
    const char* nomeArquivo = argv[1];
    std::ifstream inputFile(nomeArquivo);
    if (!inputFile.is_open()) {
        //std::cerr << "ERRO: Nao foi possivel abrir o arquivo '" << nomeArquivo << "'" << std::endl;
        return 1;
    }
    int capacidadeTransporte, numArmazens, numPacotes;
    double latenciaTransporte, intervaloTransportes, custoRemocao;
    inputFile >> capacidadeTransporte >> latenciaTransporte >> intervaloTransportes >> custoRemocao >> numArmazens;
    int** matrizAdj = new int*[numArmazens];
    for (int i = 0; i < numArmazens; ++i) {
        matrizAdj[i] = new int[numArmazens];
        for (int j = 0; j < numArmazens; ++j) inputFile >> matrizAdj[i][j];
    }
    Simulador sim(capacidadeTransporte, latenciaTransporte, intervaloTransportes, custoRemocao, numArmazens, matrizAdj);
    inputFile >> numPacotes;
    double minTempoPostagem = std::numeric_limits<double>::max();
    bool nenhumPacoteAgendado = true;
    for (int i = 0; i < numPacotes; ++i) {
        double tempoPostagem;
        int idLido, origem, destino;
        std::string s_pac, s_org, s_dst;
        inputFile >> tempoPostagem >> s_pac >> idLido >> s_org >> origem >> s_dst >> destino;
        if (tempoPostagem < minTempoPostagem) minTempoPostagem = tempoPostagem;
        nenhumPacoteAgendado = false;
        int idPacoteSequencial = i;
        Rota rota = Pathfinder::encontrarRota(origem, destino, numArmazens, matrizAdj);
        if (rota.passos == nullptr && origem != destino) {
            //std::cerr << "AVISO: Pacote " << idPacoteSequencial << " de " << origem << " para " << destino << " nao possui rota. Pacote descartado." << std::endl;
        } else {
            Pacote* novoPacote = new Pacote(idPacoteSequencial, origem, destino, tempoPostagem, rota);
            sim.adicionarPacoteInicial(novoPacote);
        }
    }
    if (!nenhumPacoteAgendado) {
        double tempoPrimeiroTransporte = minTempoPostagem + intervaloTransportes;
        sim.agendarEvento(new EventoTransporteDiario(tempoPrimeiroTransporte));
    }
    sim.executar();
    for (int i = 0; i < numArmazens; ++i) delete[] matrizAdj[i];
    delete[] matrizAdj;
    inputFile.close();
    return 0;
}