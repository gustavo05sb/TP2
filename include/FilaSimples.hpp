#ifndef FILA_SIMPLES_HPP
#define FILA_SIMPLES_HPP

/**
 * @class FilaSimples
 * @brief Implementa uma fila FIFO (First-In, First-Out) para inteiros.
 *
 * Esta estrutura de dados auxiliar é essencial para o algoritmo de Busca em Largura (BFS)
 * usado no Pathfinder para encontrar a rota mais curta. Usa um array circular.
 */
class FilaSimples {
private:
    int* dados;
    int capacidade;
    int tamanho;
    int frente; // Índice do primeiro elemento.
    int tras;   // Índice da próxima posição livre.

public:
    FilaSimples(int capacidadeInicial = 16);
    ~FilaSimples();
    
    bool estaVazia() const;
    void enfileirar(int valor); // Adiciona um item no final da fila.
    int desenfileirar();      // Remove e retorna um item do início da fila.
};

#endif // FILA_SIMPLES_HPP
