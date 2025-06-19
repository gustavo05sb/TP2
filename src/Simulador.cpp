#include "Simulador.hpp"
#include "Logger.hpp"
#include "Estruturas.hpp"
#include <iostream>

// --- Função auxiliar para saber se ainda há pacotes em armazéns ---
bool Simulador::haPacotesEmArmazens() const {
    for (int i = 0; i < numArmazens; ++i) {
        const Armazem* arm = armazens[i];
        for (int j = 0; j < arm->getNumSecoes(); ++j) {
            const SecaoArmazem* secao = arm->getSecao(j);
            if (secao->qtdPacotes > 0) return true;
        }
    }
    return false;
}

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
    for (int i = 0; i < this->numTotalPacotes; ++i) delete this->todosOsPacotes[i];
    delete[] this->todosOsPacotes;
}

void Simulador::notificarEntregaDePacote() { this->pacotesEntregues++; }
bool Simulador::todosPacotesEntregues() const { return (this->numTotalPacotes > 0) && (this->pacotesEntregues >= this->numTotalPacotes); }
double Simulador::getTempoAtual() const { return tempoAtual; }
Armazem* Simulador::getArmazem(int id) { return (id >= 0 && id < numArmazens) ? armazens[id] : nullptr; }
int Simulador::getNumArmazens() const { return numArmazens; }
int Simulador::getNumTotalPacotes() const { return numTotalPacotes; }
int Simulador::getCapacidadeTransporte() const { return capacidadeTransporte; }
double Simulador::getLatenciaTransporte() const { return latenciaTransporte; }
double Simulador::getIntervaloTransportes() const { return intervaloTransportes; }
double Simulador::getCustoRemocao() const { return custoRemocao; }

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
    while (escalonador.temEventos()) {
        Evento* proximoEvento = escalonador.proximo();
        if (proximoEvento->getTempo() > this->tempoAtual) this->tempoAtual = proximoEvento->getTempo();
        proximoEvento->processar(this);
        delete proximoEvento;
        if (todosPacotesEntregues()) {
            escalonador.limpar();
            break;
        }
    }
}

// ---------------------- EVENTOS ----------------------

EventoChegadaPacote::EventoChegadaPacote(double tempo, Pacote* p) : Evento(tempo), pacote(p) {}

void EventoChegadaPacote::processar(Simulador* sim) {
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
        // Só avança a etapa da rota após armazenar
        pacote->avancarEtapaRota();
    }
}

EventoTransporteDiario::EventoTransporteDiario(double tempo) : Evento(tempo) {}

void EventoTransporteDiario::processar(Simulador* sim) {
    if (sim->todosPacotesEntregues() || !sim->haPacotesEmArmazens()) {
        return;
    }
    double tempoAtual = sim->getTempoAtual();
    int capacidade = sim->getCapacidadeTransporte();

    for (int idOrigem = 0; idOrigem < sim->getNumArmazens(); ++idOrigem) {
        Armazem* origem = sim->getArmazem(idOrigem);
        for (int j = 0; j < origem->getNumVizinhos(); ++j) {
            int idDestino = origem->getVizinho(j);
            int quantidadeNaSecao = 0;
            Pacote** pilhaLifo = origem->pegarPacotesParaTransporteLIFO(idDestino, quantidadeNaSecao);

            if (quantidadeNaSecao == 0) {
                delete[] pilhaLifo;
                continue;
            }

            // 1. Remoção LIFO: todos os pacotes removidos da pilha (topo para base)
            double tempoRemocaoBase = tempoAtual;
            for (int k = 0; k < quantidadeNaSecao; ++k) {
                double tempoDeRemocao = tempoRemocaoBase + ((k + 1) * sim->getCustoRemocao());
                Logger::removido(tempoDeRemocao, pilhaLifo[k]->getId(), idOrigem, idDestino);
            }
            double tempoDePartida = tempoRemocaoBase + (quantidadeNaSecao * sim->getCustoRemocao());

            // 2. Transporte LIFO: do fundo da pilha (último removido) até o topo, até a capacidade
            int enviados = 0;
            for (int k = quantidadeNaSecao - 1; k >= 0 && enviados < capacidade; --k, ++enviados) {
                Logger::emTransito(tempoDePartida, pilhaLifo[k]->getId(), idOrigem, idDestino);
                pilhaLifo[k]->setIdArmazemAtual(idDestino);
                double tempoDeChegada = tempoDePartida + sim->getLatenciaTransporte();
                sim->agendarEvento(new EventoChegadaPacote(tempoDeChegada, pilhaLifo[k]));
            }

            // 3. Rearmazenamento: os que sobraram, ordenados por ID crescente
            if (enviados < quantidadeNaSecao) {
                // Copia os pacotes restantes para um array temporário
                int qtdRearmazenar = quantidadeNaSecao - enviados;
                Pacote** paraRearmazenar = new Pacote*[qtdRearmazenar];
                int idx = 0;
                for (int k = 0; k < quantidadeNaSecao - enviados; ++k)
                    paraRearmazenar[idx++] = pilhaLifo[k];

                // Ordena por ID crescente (bubble sort simples)
                for (int a = 0; a < qtdRearmazenar - 1; ++a) {
                    for (int b = 0; b < qtdRearmazenar - a - 1; ++b) {
                        if (paraRearmazenar[b]->getId() > paraRearmazenar[b+1]->getId()) {
                            Pacote* tmp = paraRearmazenar[b];
                            paraRearmazenar[b] = paraRearmazenar[b+1];
                            paraRearmazenar[b+1] = tmp;
                        }
                    }
                }

                for (int k = 0; k < qtdRearmazenar; ++k) {
                    Logger::rearmazenado(tempoDePartida, paraRearmazenar[k]->getId(), idOrigem, idDestino);
                    origem->receberPacote(paraRearmazenar[k]);
                }
                delete[] paraRearmazenar;
            }
            delete[] pilhaLifo;
        }
    }

    // Só agenda novo transporte se ainda houver pacotes em armazéns
    if (!sim->todosPacotesEntregues()) {
        double proximoTempo = sim->getTempoAtual() + sim->getIntervaloTransportes();
        sim->agendarEvento(new EventoTransporteDiario(proximoTempo));
    }
}