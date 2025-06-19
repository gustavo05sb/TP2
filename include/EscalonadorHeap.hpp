#ifndef ESCALONADOR_HEAP_HPP
#define ESCALONADOR_HEAP_HPP
#include "Evento.hpp"
#include "Estruturas.hpp" // <-- Adicione esta linha

// NÃO inclua "Simulador.hpp" aqui!

// Definição completa da classe EventoChegadaPacote (copiada de Simulador.hpp)
class EventoChegadaPacote;

class EscalonadorHeap {
private:
    Evento** heap;
    int capacidade;
    int tamanho;
    void redimensionar();
    void heapifyParaCima(int indice);
    void heapifyParaBaixo(int indice);
    int getPai(int i) { return (i - 1) / 2; }
    int getFilhoEsquerda(int i) { return 2 * i + 1; }
    int getFilhoDireita(int i) { return 2 * i + 2; }
    void trocar(int i, int j);
public:
    EscalonadorHeap(int capacidadeInicial = 32);
    ~EscalonadorHeap();
    void agendar(Evento* evento);
    void limpar();
    Evento* proximo();
    bool temEventos() const;
};
#endif