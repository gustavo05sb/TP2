#include "PilhaPacotes.hpp"
#include "Pacote.hpp" // A implementação precisa da definição completa de Pacote.

PilhaPacotes::PilhaPacotes(int capacidadeInicial) {
    this->capacidade = (capacidadeInicial > 0) ? capacidadeInicial : 8;
    this->tamanho = 0;
    this->dados = new Pacote*[this->capacidade];
}

PilhaPacotes::~PilhaPacotes() {
    // IMPORTANTE: O destrutor da pilha não deleta os pacotes que ela contém.
    // A posse dos pacotes é transferida para outras partes do sistema (como um transporte ou
    // de volta para o armazém). A pilha apenas deleta o array de ponteiros que ela mesma alocou.
    delete[] this->dados;
}

void PilhaPacotes::redimensionar() {
    this->capacidade *= 2;
    Pacote** novosDados = new Pacote*[this->capacidade];
    for (int i = 0; i < this->tamanho; ++i) {
        novosDados[i] = this->dados[i];
    }
    delete[] this->dados;
    this->dados = novosDados;
}

void PilhaPacotes::adicionar(Pacote* pacote) {
    if (this->tamanho == this->capacidade) {
        redimensionar();
    }
    this->dados[this->tamanho] = pacote;
    this->tamanho++;
}

Pacote* PilhaPacotes::remover() {
    if (estaVazia()) {
        return nullptr;
    }
    this->tamanho--;
    return this->dados[this->tamanho];
}

bool PilhaPacotes::estaVazia() const {
    return this->tamanho == 0;
}

int PilhaPacotes::getTamanho() const {
    return this->tamanho;
}