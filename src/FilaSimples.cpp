#include "FilaSimples.hpp"
#include <stdexcept> // Para std::runtime_error

FilaSimples::FilaSimples(int capacidadeInicial) {
    this->capacidade = capacidadeInicial;
    this->dados = new int[this->capacidade];
    this->tamanho = 0;
    this->frente = 0;
    this->tras = 0;
}

FilaSimples::~FilaSimples() {
    delete[] this->dados;
}

bool FilaSimples::estaVazia() const {
    return this->tamanho == 0;
}

void FilaSimples::enfileirar(int valor) {
    if (this->tamanho == this->capacidade) {
        // O Pathfinder aloca a fila com o número exato de armazéns,
        // então ela nunca deve encher. Se encher, é um erro de lógica.
        throw std::runtime_error("Fila Simples (BFS) cheia!");
    }
    this->dados[this->tras] = valor;
    this->tras = (this->tras + 1) % this->capacidade; // Lógica de array circular
    this->tamanho++;
}

int FilaSimples::desenfileirar() {
    if (estaVazia()) {
        throw std::runtime_error("Fila Simples (BFS) vazia!");
    }
    int valor = this->dados[this->frente];
    this->frente = (this->frente + 1) % this->capacidade;
    this->tamanho--;
    return valor;
}