#ifndef UTILS_HPP
#define UTILS_HPP

#include "Pacote.hpp"

/**
 * @class Utils
 * @brief Fornece funções de utilidade estáticas para o projeto.
 */
class Utils {
public:
    /**
     * @brief Ordena um array de ponteiros de Pacote com base no ID, em ordem crescente.
     * @param pacotes O array de ponteiros a ser ordenado.
     * @param tamanho O número de elementos no array.
     *
     * Usa o algoritmo Bubble Sort pela simplicidade de implementação, já que a biblioteca
     * <algorithm> não pode ser usada. Essencial para o rearmazenamento ordenado (Fase 7).
     */
    static void ordenarPacotesPorId(Pacote** pacotes, int tamanho);
};

#endif // UTILS_HPP