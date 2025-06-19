#include "Armazem.hpp"
#include "Estruturas.hpp"

Armazem::Armazem(int id, int* vizinhos, int numVizinhos) 
    : id(id), vizinhos(vizinhos), numVizinhos(numVizinhos), numSecoes(numVizinhos) {
    if (this->numSecoes > 0) {
        this->secoes = new SecaoArmazem[this->numSecoes];
        for(int i = 0; i < this->numSecoes; ++i) {
            this->secoes[i].idDestinoVizinho = this->vizinhos[i];
            this->secoes[i].qtdPacotes = 0; // Garante inicialização
        }
    } else {
        this->secoes = nullptr;
    }
}

Armazem::~Armazem() {
    delete[] this->vizinhos;
    delete[] this->secoes;
}

// --- Getters ---
int Armazem::getId() const { return this->id; }
int Armazem::getNumVizinhos() const { return this->numVizinhos; }
int Armazem::getVizinho(int i) const { return (i >= 0 && i < this->numVizinhos) ? this->vizinhos[i] : -1; }

// --- Lógica do Armazém ---
SecaoArmazem* Armazem::encontrarSecao(int idDestino) {
    for (int i = 0; i < this->numSecoes; ++i) {
        if (this->secoes[i].idDestinoVizinho == idDestino) {
            return &this->secoes[i];
        }
    }
    return nullptr;
}

void Armazem::receberPacote(Pacote* pacote) {
    int proximoDestino = pacote->getProximoDestino();
    if (proximoDestino == -1) return;
    SecaoArmazem* secao = encontrarSecao(proximoDestino);
    if (secao && secao->qtdPacotes < MAX_PACOTES_SECAO) {
        secao->pacotes[secao->qtdPacotes++] = pacote;
    }
}

// --- NOVO: Remoção LIFO de todos os pacotes da seção ---
Pacote** Armazem::pegarPacotesParaTransporteLIFO(int idDestino, int& quantidadePegada) {
    quantidadePegada = 0;
    SecaoArmazem* secao = encontrarSecao(idDestino);
    if (!secao || secao->qtdPacotes == 0)
        return nullptr;

    int n = secao->qtdPacotes;
    Pacote** pilhaLifo = new Pacote*[n];
    // Remove todos os pacotes da pilha (LIFO: topo para base)
    for (int k = 0; k < n; ++k) {
        pilhaLifo[k] = secao->pacotes[secao->qtdPacotes - 1 - k];
    }
    secao->qtdPacotes = 0;
    quantidadePegada = n;
    return pilhaLifo;
}