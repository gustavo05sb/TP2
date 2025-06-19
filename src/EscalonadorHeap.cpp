#include "EscalonadorHeap.hpp"

// Função helper para comparação, que usa o ID do evento como desempate.
// Garante uma ordem de processamento determinística para eventos simultâneos.
bool eventoMenorQue(Evento* a, Evento* b) {
    if (a->getTempo() < b->getTempo()) return true;
    if (a->getTempo() > b->getTempo()) return false;
    // Tempos iguais, desempata pelo ID do evento (o menor ID veio primeiro).
    return a->getId() < b->getId();
}

EscalonadorHeap::EscalonadorHeap(int capacidadeInicial) {
    this->capacidade = capacidadeInicial;
    this->tamanho = 0;
    this->heap = new Evento*[this->capacidade];
}

EscalonadorHeap::~EscalonadorHeap() {
    // Deleta quaisquer eventos que permaneceram na fila ao final da simulação.
    for (int i = 0; i < this->tamanho; i++) {
        delete this->heap[i];
    }
    delete[] this->heap;
}

void EscalonadorHeap::trocar(int i, int j) {
    Evento* temp = this->heap[i];
    this->heap[i] = this->heap[j];
    this->heap[j] = temp;
}

void EscalonadorHeap::redimensionar() {
    this->capacidade *= 2;
    Evento** novoHeap = new Evento*[this->capacidade];
    for(int i = 0; i < this->tamanho; ++i) {
        novoHeap[i] = this->heap[i];
    }
    delete[] this->heap;
    this->heap = novoHeap;
}

void EscalonadorHeap::heapifyParaCima(int indice) {
    int pai = getPai(indice);
    if (indice > 0 && eventoMenorQue(this->heap[indice], this->heap[pai])) {
        trocar(indice, pai);
        heapifyParaCima(pai);
    }
}

void EscalonadorHeap::heapifyParaBaixo(int indice) {
    int esq = getFilhoEsquerda(indice);
    int dir = getFilhoDireita(indice);
    int menor = indice;

    if (esq < this->tamanho && eventoMenorQue(this->heap[esq], this->heap[menor])) {
        menor = esq;
    }
    if (dir < this->tamanho && eventoMenorQue(this->heap[dir], this->heap[menor])) {
        menor = dir;
    }

    if (menor != indice) {
        trocar(indice, menor);
        heapifyParaBaixo(menor);
    }
}

void EscalonadorHeap::agendar(Evento* evento) {
    if (this->tamanho == this->capacidade) {
        redimensionar();
    }
    this->heap[this->tamanho] = evento;
    this->tamanho++;
    heapifyParaCima(this->tamanho - 1);
}

Evento* EscalonadorHeap::proximo() {
    if (this->tamanho == 0) {
        return nullptr;
    }
    Evento* raiz = this->heap[0];
    this->heap[0] = this->heap[this->tamanho - 1];
    this->tamanho--;
    heapifyParaBaixo(0);
    return raiz;
}

bool EscalonadorHeap::temEventos() const {
    return this->tamanho > 0;
}