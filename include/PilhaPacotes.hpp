#ifndef PILHA_PACOTES_HPP
#define PILHA_PACOTES_HPP

// Forward declaration para evitar dependência circular com Pacote.hpp.
// A definição completa não é necessária no cabeçalho.
class Pacote; 

/**
 * @class PilhaPacotes
 * @brief Implementa uma pilha LIFO (Last-In, First-Out) para ponteiros de Pacote.
 *
 * Esta estrutura é usada dentro das seções de um Armazem para guardar pacotes.
 * Usa um array dinâmico e redimensiona quando necessário.
 * A posse dos ponteiros de Pacote não pertence à pilha; ela apenas os armazena.
 */
class PilhaPacotes {
private:
    Pacote** dados; // Array dinâmico de ponteiros para Pacote.
    int tamanho;    // Número atual de pacotes na pilha.
    int capacidade; // Tamanho alocado do array 'dados'.

    // Função helper para dobrar a capacidade do array quando estiver cheio.
    void redimensionar();

public:
    // Construtor: inicializa a pilha com uma capacidade inicial.
    PilhaPacotes(int capacidadeInicial = 8);
    
    // Destrutor: libera a memória do array de ponteiros, mas não os pacotes em si.
    ~PilhaPacotes();

    // Adiciona um ponteiro de Pacote ao topo da pilha (push).
    void adicionar(Pacote* pacote);
    
    // Remove e retorna o ponteiro de Pacote do topo da pilha (pop).
    Pacote* remover();
    
    // Verifica se a pilha está vazia.
    bool estaVazia() const;
    
    // Retorna o número de pacotes na pilha.
    int getTamanho() const;
};

#endif // PILHA_PACOTES_HPP