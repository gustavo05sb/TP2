#ifndef SIMULADOR_HPP
#define SIMULADOR_HPP

#include "EscalonadorHeap.hpp"
#include "Armazem.hpp"
#include "Pacote.hpp"

/**
 * @class Simulador
 * @brief A classe principal que orquestra toda a simulação.
 *
 * Contém o estado global do sistema (tempo, armazéns, pacotes) e o motor
 * de execução (o laço principal que processa eventos do escalonador).
 */
class Simulador {
private:
    double tempoAtual;
    EscalonadorHeap escalonador;

    // Parâmetros de simulação lidos do arquivo.
    int capacidadeTransporte;
    double latenciaTransporte;
    double intervaloTransportes;
    double custoRemocao;
    int numArmazens;

    // Estruturas do sistema.
    Armazem** armazens;
    Pacote** todosOsPacotes; // Lista mestra para referência e limpeza de memória.
    int numTotalPacotes;
    int capacidadePacotes;
    int pacotesEntregues; // Contador para a condição de parada.

    // Função helper para adicionar um pacote à lista mestra.
    void adicionarPacoteNaListaMestra(Pacote* p);

public:
    Simulador(int capTransp, double latTransp, double interTransp, double custoRem, int nArmazens, int** matrizAdj);
    ~Simulador();

    void executar();
    void agendarEvento(Evento* ev);
    void adicionarPacoteInicial(Pacote* p);
    void notificarEntregaDePacote();
    bool todosPacotesEntregues() const;

    // Getters para os parâmetros e estado do sistema.
    double getTempoAtual() const { return tempoAtual; }
    Armazem* getArmazem(int id) { return (id >= 0 && id < numArmazens) ? armazens[id] : nullptr; }
    int getNumArmazens() const { return numArmazens; }
    int getNumTotalPacotes() const { return numTotalPacotes; }
    int getCapacidadeTransporte() const { return capacidadeTransporte; }
    double getLatenciaTransporte() const { return latenciaTransporte; }
    double getIntervaloTransportes() const { return intervaloTransportes; }
    double getCustoRemocao() const { return custoRemocao; }
};

// --- Classes de Evento Derivadas ---

class EventoChegadaPacote : public Evento {
private:
    Pacote* pacote;
public:
    EventoChegadaPacote(double tempo, Pacote* p);
    virtual void processar(Simulador* sim) override;
    virtual TipoEvento getTipo() const override { return CHEGADA_PACOTE; }
};

class EventoTransporteDiario : public Evento {
public:
    EventoTransporteDiario(double tempo);
    virtual void processar(Simulador* sim) override;
    virtual TipoEvento getTipo() const override { return TRANSPORTE_DIARIO; }
};

#endif // SIMULADOR_HPP