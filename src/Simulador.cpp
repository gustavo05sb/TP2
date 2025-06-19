#include "Simulador.hpp"
#include "Logger.hpp"
#include "Utils.hpp"
#include <iostream>

// --- IMPLEMENTAÇÃO DO SIMULADOR ---
Simulador::Simulador(int capTransp, double latTransp, double interTransp, double custoRem, int nArmazens, int** matrizAdj) 
    : tempoAtual(0.0), capacidadeTransporte(capTransp), latenciaTransporte(latTransp), 
      intervaloTransportes(interTransp), custoRemocao(custoRem), numArmazens(nArmazens) {
    
    this->pacotesEntregues = 0;
    this->capacidadePacotes = 64;
    this->numTotalPacotes = 0;
    this->todosOsPacotes = new Pacote*[this->capacidadePacotes];

    this->armazens = new Armazem*[this->numArmazens];
    for (int i = 0; i < this->numArmazens; ++i) {
        int vizinhosCount = 0;
        for (int j = 0; j < this->numArmazens; ++j) if (matrizAdj[i][j] == 1) vizinhosCount++;
        
        int* listaVizinhos = new int[vizinhosCount];
        int k = 0;
        for (int j = 0; j < this->numArmazens; ++j) if (matrizAdj[i][j] == 1) listaVizinhos[k++] = j;
        this->armazens[i] = new Armazem(i, listaVizinhos, vizinhosCount);
    }
}

Simulador::~Simulador() {
    for (int i = 0; i < this->numArmazens; ++i) delete this->armazens[i];
    delete[] this->armazens;

    // A lista mestra é a "dona" de todos os pacotes, então ela os deleta.
    for (int i = 0; i < this->numTotalPacotes; ++i) delete this->todosOsPacotes[i];
    delete[] this->todosOsPacotes;
}

void Simulador::notificarEntregaDePacote() { this->pacotesEntregues++; }

bool Simulador::todosPacotesEntregues() const { return this->pacotesEntregues >= this->numTotalPacotes; }

void Simulador::adicionarPacoteNaListaMestra(Pacote* p) {
    if (this->numTotalPacotes == this->capacidadePacotes) {
        this->capacidadePacotes *= 2;
        Pacote** novaLista = new Pacote*[this->capacidadePacotes];
        for(int i = 0; i < this->numTotalPacotes; ++i) novaLista[i] = this->todosOsPacotes[i];
        delete[] this->todosOsPacotes;
        this->todosOsPacotes = novaLista;
    }
    this->todosOsPacotes[this->numTotalPacotes++] = p;
}

void Simulador::adicionarPacoteInicial(Pacote* p) {
    adicionarPacoteNaListaMestra(p);
    agendarEvento(new EventoChegadaPacote(p->getTempoPostagem(), p));
}

void Simulador::agendarEvento(Evento* ev) { this->escalonador.agendar(ev); }

void Simulador::executar() {
    std::cout << "\n--- INICIANDO EXECUCAO DA SIMULACAO ---\n" << std::endl;
    const long long LIMITE_MAX_ITERACOES = 1000000;
    long long iteracoes = 0;

    while (escalonador.temEventos() && !todosPacotesEntregues() && iteracoes < LIMITE_MAX_ITERACOES) {
        Evento* proximoEvento = escalonador.proximo();
        if (proximoEvento->getTempo() > this->tempoAtual) this->tempoAtual = proximoEvento->getTempo();
        proximoEvento->processar(this);
        delete proximoEvento; // O evento é usado e descartado.
        iteracoes++;
    }
    
    std::cout << "\n--- SIMULACAO CONCLUIDA NO TEMPO " << static_cast<int>(this->tempoAtual) << " ---\n" << std::endl;
    std::cout << "Relatorio Final:" << std::endl;
    std::cout << " - Eventos processados: " << iteracoes << std::endl;
    std::cout << " - Pacotes entregues: " << this->pacotesEntregues << " de " << this->numTotalPacotes << std::endl;
    if (iteracoes >= LIMITE_MAX_ITERACOES) std::cerr << "AVISO: Simulacao interrompida por atingir o limite maximo de iteracoes!" << std::endl;
    else if (todosPacotesEntregues()) std::cout << "INFO: Simulacao encerrada com sucesso. Todos os pacotes foram entregues." << std::endl;
    else std::cout << "AVISO: Simulacao encerrada, mas " << this->numTotalPacotes - this->pacotesEntregues << " pacotes nao foram entregues." << std::endl;
}

// --- IMPLEMENTAÇÃO DOS EVENTOS ---
EventoChegadaPacote::EventoChegadaPacote(double tempo, Pacote* p) : Evento(tempo), pacote(p) {}

void EventoChegadaPacote::processar(Simulador* sim) {
    pacote->avancarEtapaRota();
    int idArmazemChegada = pacote->getIdArmazemAtual();

    if (pacote->chegouAoDestinoFinal()) {
        Logger::entregue(sim->getTempoAtual(), pacote->getId(), idArmazemChegada);
        sim->notificarEntregaDePacote();
    } else {
        Armazem* armazem = sim->getArmazem(idArmazemChegada);
        if (armazem) {
            int proximoDestino = pacote->getProximoDestino();
            Logger::armazenado(sim->getTempoAtual(), pacote->getId(), idArmazemChegada, proximoDestino);
            armazem->receberPacote(pacote);
        }
    }
}

EventoTransporteDiario::EventoTransporteDiario(double tempo) : Evento(tempo) {}

void EventoTransporteDiario::processar(Simulador* sim) {
    double tempoAtual = sim->getTempoAtual();
    for (int idOrigem = 0; idOrigem < sim->getNumArmazens(); ++idOrigem) {
        Armazem* origem = sim->getArmazem(idOrigem);
        for (int j = 0; j < origem->getNumVizinhos(); ++j) {
            int idDestino = origem->getVizinho(j);
            int quantidadeNaSecao = 0;
            Pacote** pacotesRemovidos = origem->pegarPacotesParaTransporte(idDestino, 9999, quantidadeNaSecao);

            if (quantidadeNaSecao == 0) continue;

            double tempoDePartida = tempoAtual;
            for (int k = 0; k < quantidadeNaSecao; k++) {
                double tempoDeRemocao = tempoAtual + (k * sim->getCustoRemocao());
                Logger::removido(tempoDeRemocao, pacotesRemovidos[k]->getId(), idOrigem, idDestino);
                tempoDePartida = tempoDeRemocao;
            }
            
            int capacidade = sim->getCapacidadeTransporte();
            int pacotesParaTransportar = (quantidadeNaSecao < capacidade) ? quantidadeNaSecao : capacidade;
            
            for (int k = 0; k < pacotesParaTransportar; ++k) {
                Pacote* pacoteViajante = pacotesRemovidos[k]; // Política FIFO
                Logger::emTransito(tempoDePartida, pacoteViajante->getId(), idOrigem, idDestino);
                pacoteViajante->setIdArmazemAtual(idDestino);
                double tempoDeChegada = tempoDePartida + sim->getLatenciaTransporte();
                sim->agendarEvento(new EventoChegadaPacote(tempoDeChegada, pacoteViajante));
            }
            
            int pacotesParaRearmazenar = quantidadeNaSecao - pacotesParaTransportar;
            if (pacotesParaRearmazenar > 0) {
                Pacote** sobraram = &pacotesRemovidos[pacotesParaTransportar];
                Utils::ordenarPacotesPorId(sobraram, pacotesParaRearmazenar);
                for (int k = 0; k < pacotesParaRearmazenar; ++k) {
                    Logger::rearmazenado(tempoDePartida, sobraram[k]->getId(), idOrigem, idDestino);
                    origem->receberPacote(sobraram[k]);
                }
            }
            delete[] pacotesRemovidos;
        }
    }
    
    if (!sim->todosPacotesEntregues()) {
        double proximoTempo = sim->getTempoAtual() + sim->getIntervaloTransportes();
        sim->agendarEvento(new EventoTransporteDiario(proximoTempo));
    }
}