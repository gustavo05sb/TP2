#ifndef ESTRUTURAS_HPP
#define ESTRUTURAS_HPP

#include <iostream>

// ----------- PathFinder e Rota -----------
struct Rota {
    int* passos;
    int numPassos;
};

class Pathfinder {
public:
    static Rota encontrarRota(int origem, int destino, int numArmazens, int** matrizAdj);
};

// ----------- Pacote -----------
class Pacote {
private:
    int id;
    int idArmazemOrigem;
    int idArmazemDestinoFinal;
    double tempoDePostagem;
    int idArmazemAtual;
    Rota rota;
    int etapaAtualDaRota;
public:
    Pacote(int id, int origem, int destino, double tempoPostagem, Rota rotaCalculada);
    Pacote(const Pacote& outro);
    Pacote& operator=(const Pacote& outro);
    ~Pacote();
    int getId() const;
    int getIdArmazemAtual() const;
    double getTempoPostagem() const;
    int getProximoDestino() const;
    void setIdArmazemAtual(int id);
    void avancarEtapaRota();
    bool chegouAoDestinoFinal() const;
};

// ----------- Utils -----------
class Utils {
public:
    static void ordenarPacotesPorId(Pacote** pacotes, int tamanho);
};

// ----------- FilaSimples -----------
class FilaSimples {
private:
    int* dados;
    int capacidade;
    int tamanho;
    int frente;
    int tras;
public:
    FilaSimples(int capacidadeInicial = 16);
    ~FilaSimples();
    bool estaVazia() const;
    void enfileirar(int valor);
    int desenfileirar();
};

// ----------- PilhaPacotes -----------
class PilhaPacotes {
private:
    Pacote** dados;
    int tamanho;
    int capacidade;
    void redimensionar();
public:
    PilhaPacotes(int capacidadeInicial = 8);
    ~PilhaPacotes();
    void adicionar(Pacote* pacote);
    Pacote* remover();
    bool estaVazia() const;
    int getTamanho() const;
};

// ----------- PilhaOuFila -----------
enum PoliticaSecao { FIFO, LIFO };

class PilhaOuFila {
    Pacote** dados;
    int tamanho;
    int capacidade;
    PoliticaSecao politica;
    void redimensionar() {
        int novaCapacidade = capacidade * 2;
        Pacote** novo = new Pacote*[novaCapacidade];
        for (int i = 0; i < tamanho; ++i) novo[i] = dados[i];
        delete[] dados;
        dados = novo;
        capacidade = novaCapacidade;
    }
public:
    PilhaOuFila(int capacidadeInicial = 8) : tamanho(0), capacidade(capacidadeInicial), politica(FIFO) {
        dados = new Pacote*[capacidade];
    }
    ~PilhaOuFila() { delete[] dados; }
    void setPolitica(PoliticaSecao pol) { politica = pol; }
    void adicionar(Pacote* p) {
        if (tamanho == capacidade) redimensionar();
        dados[tamanho++] = p;
    }
    Pacote* remover() {
        if (tamanho == 0) return nullptr;
        if (politica == FIFO) {
            Pacote* p = dados[0];
            for (int i = 1; i < tamanho; ++i) dados[i-1] = dados[i];
            tamanho--;
            return p;
        } else { // LIFO
            return dados[--tamanho];
        }
    }
    bool estaVazia() const { return tamanho == 0; }
    int getTamanho() const { return tamanho; }
};

#endif // ESTRUTURAS_HPP