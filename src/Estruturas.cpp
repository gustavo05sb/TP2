#include "Estruturas.hpp"
#include <stdexcept>

// ----------- FilaSimples -----------
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
        throw std::runtime_error("Fila Simples (BFS) cheia!");
    }
    this->dados[this->tras] = valor;
    this->tras = (this->tras + 1) % this->capacidade;
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

// ----------- PilhaPacotes -----------
PilhaPacotes::PilhaPacotes(int capacidadeInicial) {
    this->capacidade = (capacidadeInicial > 0) ? capacidadeInicial : 8;
    this->tamanho = 0;
    this->dados = new Pacote*[this->capacidade];
}
PilhaPacotes::~PilhaPacotes() {
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

// ----------- PilhaOuFila -----------
// (Implementação inline no header, não precisa duplicar aqui)

// ----------- Pacote -----------
Pacote::Pacote(int id, int origem, int destino, double tempoPostagem, Rota rotaCalculada)
    : id(id), idArmazemOrigem(origem), idArmazemDestinoFinal(destino), 
      tempoDePostagem(tempoPostagem), idArmazemAtual(origem),
      rota(rotaCalculada), etapaAtualDaRota(0) {}

Pacote::~Pacote() {
    delete[] this->rota.passos;
}

Pacote::Pacote(const Pacote& outro) {
    this->id = outro.id;
    this->idArmazemOrigem = outro.idArmazemOrigem;
    this->idArmazemDestinoFinal = outro.idArmazemDestinoFinal;
    this->tempoDePostagem = outro.tempoDePostagem;
    this->idArmazemAtual = outro.idArmazemAtual;
    this->etapaAtualDaRota = outro.etapaAtualDaRota;
    
    this->rota.numPassos = outro.rota.numPassos;
    if (this->rota.numPassos > 0) {
        this->rota.passos = new int[this->rota.numPassos];
        for(int i = 0; i < this->rota.numPassos; ++i) {
            this->rota.passos[i] = outro.rota.passos[i];
        }
    } else {
        this->rota.passos = nullptr;
    }
}

Pacote& Pacote::operator=(const Pacote& outro) {
    if (this == &outro) return *this;
    delete[] this->rota.passos;
    this->id = outro.id;
    this->idArmazemOrigem = outro.idArmazemOrigem;
    this->idArmazemDestinoFinal = outro.idArmazemDestinoFinal;
    this->tempoDePostagem = outro.tempoDePostagem;
    this->idArmazemAtual = outro.idArmazemAtual;
    this->etapaAtualDaRota = outro.etapaAtualDaRota;
    this->rota.numPassos = outro.rota.numPassos;
    if (this->rota.numPassos > 0) {
        this->rota.passos = new int[this->rota.numPassos];
        for(int i = 0; i < this->rota.numPassos; ++i) {
            this->rota.passos[i] = outro.rota.passos[i];
        }
    } else {
        this->rota.passos = nullptr;
    }
    return *this;
}

int Pacote::getId() const { return this->id; }
int Pacote::getIdArmazemAtual() const { return this->idArmazemAtual; }
double Pacote::getTempoPostagem() const { return this->tempoDePostagem; }
void Pacote::setIdArmazemAtual(int id) { this->idArmazemAtual = id; }

int Pacote::getProximoDestino() const {
    if (this->etapaAtualDaRota < this->rota.numPassos) {
        return this->rota.passos[this->etapaAtualDaRota];
    }
    return this->idArmazemDestinoFinal;
}

void Pacote::avancarEtapaRota() {
    if (this->etapaAtualDaRota < this->rota.numPassos) {
        this->etapaAtualDaRota++;
    }
}

bool Pacote::chegouAoDestinoFinal() const {
    return this->idArmazemAtual == this->idArmazemDestinoFinal;
}

// ----------- Pathfinder -----------
Rota Pathfinder::encontrarRota(int origem, int destino, int numArmazens, int** matrizAdj) {
    if (origem == destino) return {nullptr, 0};
    FilaSimples fila(numArmazens);
    bool* visitado = new bool[numArmazens];
    int* predecessor = new int[numArmazens];
    for (int i = 0; i < numArmazens; ++i) {
        visitado[i] = false;
        predecessor[i] = -1;
    }
    fila.enfileirar(origem);
    visitado[origem] = true;
    while (!fila.estaVazia()) {
        int u = fila.desenfileirar();
        if (u == destino) break;
        for (int v = 0; v < numArmazens; ++v) {
            if (matrizAdj[u][v] == 1 && !visitado[v]) {
                visitado[v] = true;
                predecessor[v] = u;
                fila.enfileirar(v);
            }
        }
    }
    Rota rota = {nullptr, 0};
    if (predecessor[destino] != -1) {
        int cont = 0;
        int atual = destino;
        while (predecessor[atual] != -1) {
            cont++;
            atual = predecessor[atual];
        }
        rota.numPassos = cont;
        rota.passos = new int[cont];
        atual = destino;
        for (int i = cont - 1; i >= 0; i--) {
            rota.passos[i] = atual;
            atual = predecessor[atual];
        }
    }
    delete[] visitado;
    delete[] predecessor;
    return rota;
}

// ----------- Utils -----------
void Utils::ordenarPacotesPorId(Pacote** pacotes, int tamanho) {
    if (tamanho < 2) return;
    bool trocou;
    do {
        trocou = false;
        for (int i = 0; i < tamanho - 1; ++i) {
            if (pacotes[i]->getId() > pacotes[i + 1]->getId()) {
                Pacote* temp = pacotes[i];
                pacotes[i] = pacotes[i + 1];
                pacotes[i + 1] = temp;
                trocou = true;
            }
        }
        } while (trocou);
}