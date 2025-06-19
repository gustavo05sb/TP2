#ifndef ARMAZEM_HPP
#define ARMAZEM_HPP

#include "Estruturas.hpp"

#define MAX_PACOTES_SECAO 128

struct SecaoArmazem {
    int idDestinoVizinho;
    Pacote* pacotes[MAX_PACOTES_SECAO];
    int qtdPacotes = 0;
};

class Armazem {
private:
    int id;
    int* vizinhos;
    int numVizinhos;
    SecaoArmazem* secoes;
    int numSecoes;
    SecaoArmazem* encontrarSecao(int idDestino);
public:
    Armazem(int id, int* vizinhos, int numVizinhos);
    ~Armazem();
    int getId() const;
    int getNumVizinhos() const;
    int getVizinho(int i) const;
    void receberPacote(Pacote* pacote);

    // NOVO: Remoção LIFO de todos os pacotes da seção
    Pacote** pegarPacotesParaTransporteLIFO(int idDestino, int& quantidadePegada);

    // Getters para acesso seguro às seções
    const SecaoArmazem* getSecao(int idx) const { return &secoes[idx]; }
    int getNumSecoes() const { return numSecoes; }
};

#endif // ARMAZEM_HPP