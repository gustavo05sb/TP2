#ifndef ARMAZEM_HPP
#define ARMAZEM_HPP

#include "PilhaPacotes.hpp"

// Representa uma seção de armazenamento dentro de um armazém,
// dedicada a pacotes que vão para um destino vizinho específico.
struct SecaoArmazem {
    int idDestinoVizinho;
    PilhaPacotes pilha;
};

/**
 * @class Armazem
 * @brief Representa um armazém físico na rede.
 *
 * Cada armazém tem um ID, uma lista de vizinhos diretos e um conjunto de seções
 * de armazenamento, onde cada seção é uma pilha LIFO para pacotes destinados a um vizinho.
 */
class Armazem {
private:
    int id;
    int* vizinhos;
    int numVizinhos;

    SecaoArmazem* secoes; // Array dinâmico de seções, uma para cada vizinho.
    int numSecoes; 

    // Função helper para encontrar a seção correta para um dado destino.
    SecaoArmazem* encontrarSecao(int idDestino);

public:
    // Construtor: cria um armazém com seu ID e a lista de seus vizinhos.
    Armazem(int id, int* vizinhos, int numVizinhos);
    
    // Destrutor: libera a memória dos arrays 'vizinhos' e 'secoes'.
    ~Armazem();

    int getId() const { return id; }
    int getNumVizinhos() const { return numVizinhos; }
    int getVizinho(int i) const { return (i < numVizinhos) ? vizinhos[i] : -1; }

    // Coloca um pacote na seção de armazenamento correta.
    void receberPacote(Pacote* pacote);
    
    /**
     * @brief Retira pacotes de uma seção específica.
     * @param idDestino O destino que define a seção.
     * @param capacidade A quantidade máxima a ser retirada (usado com valor alto para esvaziar).
     * @param quantidadePegada Referência para retornar o número de pacotes efetivamente retirados.
     * @return Um novo array dinâmico com os ponteiros dos pacotes retirados.
     */
    Pacote** pegarPacotesParaTransporte(int idDestino, int capacidade, int& quantidadePegada);
};

#endif // ARMAZEM_HPP