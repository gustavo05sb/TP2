#ifndef PATHFINDER_HPP
#define PATHFINDER_HPP

// Estrutura simples para encapsular o resultado da busca de rota.
// O ponteiro 'passos' deve ser desalocado pelo chamador.
struct Rota {
    int* passos;
    int numPassos;
};

/**
 * @class Pathfinder
 * @brief Fornece algoritmos para cálculo de rotas na rede de armazéns.
 */
class Pathfinder {
public:
    /**
     * @brief Encontra o caminho mais curto entre dois armazéns usando Busca em Largura (BFS).
     * @param origem O ID do armazém inicial.
     * @param destino O ID do armazém final.
     * @param numArmazens O número total de armazéns no sistema.
     * @param matrizAdj A matriz de adjacência que define as conexões.
     * @return Uma struct Rota contendo os passos do caminho. Se não houver caminho,
     * retorna {nullptr, 0}.
     */
    static Rota encontrarRota(int origem, int destino, int numArmazens, int** matrizAdj);
};

#endif // PATHFINDER_HPP