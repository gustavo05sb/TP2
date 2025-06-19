#include "Armazem.hpp"
#include "Pacote.hpp"
#include <iostream>

Armazem::Armazem(int id, int* vizinhos, int numVizinhos) 
    : id(id), vizinhos(vizinhos), numVizinhos(numVizinhos), numSecoes(numVizinhos) {
    
    // Cria uma seção para cada vizinho.
    if (this->numSecoes > 0) {
        this->secoes = new SecaoArmazem[this->numSecoes];
        for(int i = 0; i < this->numSecoes; ++i) {
            this->secoes[i].idDestinoVizinho = this->vizinhos[i];
            // A pilha dentro da seção é inicializada por seu próprio construtor padrão.
        }
    } else {
        this->secoes = nullptr;
    }
}

Armazem::~Armazem() {
    delete[] this->vizinhos;
    delete[] this->secoes;
}

// Encontra a seção correspondente a um destino.
SecaoArmazem* Armazem::encontrarSecao(int idDestino) {
    for (int i = 0; i < this->numSecoes; ++i) {
        if (this->secoes[i].idDestinoVizinho == idDestino) {
            return &this->secoes[i];
        }
    }
    return nullptr; // Não deveria acontecer se a rota for válida.
}

// Coloca um pacote na seção correta, baseada no seu próximo destino.
void Armazem::receberPacote(Pacote* pacote) {
    int proximoDestino = pacote->getProximoDestino();
    if (proximoDestino == -1) return; 

    SecaoArmazem* secao = encontrarSecao(proximoDestino);
    if (secao) {
        secao->pilha.adicionar(pacote);
    }
}

// Retira pacotes de uma seção, respeitando a capacidade.
Pacote** Armazem::pegarPacotesParaTransporte(int idDestino, int capacidade, int& quantidadePegada) {
    quantidadePegada = 0;
    SecaoArmazem* secao = encontrarSecao(idDestino);
    if (secao == nullptr || secao->pilha.estaVazia()) {
        return nullptr;
    }

    int pacotesDisponiveis = secao->pilha.getTamanho();
    quantidadePegada = (pacotesDisponiveis < capacidade) ? pacotesDisponiveis : capacidade;

    if (quantidadePegada == 0) return nullptr;

    Pacote** pacotesParaEnviar = new Pacote*[quantidadePegada];
    for (int i = 0; i < quantidadePegada; ++i) {
        // A remoção da pilha é LIFO, mas aqui simplesmente esvaziamos para a lógica de transporte decidir.
        pacotesParaEnviar[i] = secao->pilha.remover(); 
    }
    
    return pacotesParaEnviar;
}