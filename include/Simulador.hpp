#ifndef SIMULADOR_HPP
#define SIMULADOR_HPP
#include "EscalonadorHeap.hpp"
#include "Armazem.hpp"
#include "Estruturas.hpp"

class Simulador {
private:
    double tempoAtual;
    EscalonadorHeap escalonador;
    int capacidadeTransporte;
    double latenciaTransporte;
    double intervaloTransportes;
    double custoRemocao;
    int numArmazens;
    Armazem** armazens;
    Pacote** todosOsPacotes;
    int numTotalPacotes;
    int capacidadePacotes;
    int pacotesEntregues;
    void adicionarPacoteNaListaMestra(Pacote* p);
public:
    Simulador(int capTransp, double latTransp, double interTransp, double custoRem, int nArmazens, int** matrizAdj);
    ~Simulador();
    void executar();
    void agendarEvento(Evento* ev);
    void adicionarPacoteInicial(Pacote* p);
    void notificarEntregaDePacote();
    bool todosPacotesEntregues() const;
    double getTempoAtual() const;
    Armazem* getArmazem(int id);
    int getNumArmazens() const;
    int getNumTotalPacotes() const;
    int getCapacidadeTransporte() const;
    double getLatenciaTransporte() const;
    double getIntervaloTransportes() const;
    double getCustoRemocao() const;

    // Função auxiliar para evitar loop infinito em rearmazenamento
    bool haPacotesEmArmazens() const;
};

class EventoChegadaPacote : public Evento {
private:
    Pacote* pacote;
public:
    EventoChegadaPacote(double tempo, Pacote* p);
    virtual void processar(Simulador* sim) override;
    virtual TipoEvento getTipo() const override { return CHEGADA_PACOTE; }
    Pacote* getPacote() const { return pacote; }
};

class EventoTransporteDiario : public Evento {
public:
    EventoTransporteDiario(double tempo);
    virtual void processar(Simulador* sim) override;
    virtual TipoEvento getTipo() const override { return TRANSPORTE_DIARIO; }
    };
    #endif // SIMULADOR_HPP