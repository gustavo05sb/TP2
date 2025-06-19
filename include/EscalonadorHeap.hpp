#ifndef ESCALONADOR_HEAP_HPP
#define ESCALONADOR_HEAP_HPP

#include "Evento.hpp"

/**
 * @class EscalonadorHeap
 * @brief O coração do simulador. Mantém os eventos em uma Min-Heap.
 *
 * Uma Min-Heap garante que o próximo evento a ser processado (aquele com o menor
 * tempo) possa ser encontrado e removido eficientemente (em tempo O(log N)).
 * Esta é a estrutura de dados canônica para uma fila de prioridade em
 * simulação de eventos discretos.
 */
class EscalonadorHeap {
private:
    Evento** heap; // O array que representa a heap.
    int capacidade;
    int tamanho;

    // Funções helper para a manipulação da heap.
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

    void agendar(Evento* evento); // Insere um novo evento.
    Evento* proximo();            // Remove e retorna o evento de maior prioridade.
    bool temEventos() const;
};

#endif // ESCALONADOR_HEAP_HPP