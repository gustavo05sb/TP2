#include "Pacote.hpp"

Pacote::Pacote(int id, int origem, int destino, double tempoPostagem, Rota rotaCalculada)
    : id(id), idArmazemOrigem(origem), idArmazemDestinoFinal(destino), 
      tempoDePostagem(tempoPostagem), idArmazemAtual(origem),
      rota(rotaCalculada), etapaAtualDaRota(0) {
    // O construtor recebe a Rota já calculada pelo Pathfinder.
}

Pacote::~Pacote() {
    // Libera a memória alocada para o array de passos da rota.
    delete[] this->rota.passos;
}

// Implementação do construtor de cópia
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

// Implementação do operador de atribuição
Pacote& Pacote::operator=(const Pacote& outro) {
    if (this == &outro) return *this;
    
    delete[] this->rota.passos; // Libera memória antiga

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

// Retorna o próximo armazém na rota do pacote.
int Pacote::getProximoDestino() const {
    if (this->etapaAtualDaRota < this->rota.numPassos) {
        return this->rota.passos[this->etapaAtualDaRota];
    }
    return this->idArmazemDestinoFinal; // Se não há mais passos, o próximo é o final.
}

// Move o "ponteiro" interno para a próxima etapa da rota.
void Pacote::avancarEtapaRota() {
    if (this->etapaAtualDaRota < this->rota.numPassos) {
        this->etapaAtualDaRota++;
    }
}

// Verifica se o pacote já completou todas as etapas da sua rota.
bool Pacote::chegouAoDestinoFinal() const {
    // Chegou ao destino se o armazém atual for o destino final
    // E todas as etapas da rota foram concluídas.
    return this->idArmazemAtual == this->idArmazemDestinoFinal;
}