#include "Pathfinder.hpp"
#include "FilaSimples.hpp"
#include <stdexcept>

Rota Pathfinder::encontrarRota(int origem, int destino, int numArmazens, int** matrizAdj) {
    if (origem == destino) return {nullptr, 0};

    // Estruturas de dados auxiliares para o BFS
    FilaSimples fila(numArmazens);
    bool* visitado = new bool[numArmazens];
    int* predecessor = new int[numArmazens];

    // Inicialização
    for (int i = 0; i < numArmazens; ++i) {
        visitado[i] = false;
        predecessor[i] = -1;
    }

    // Começa a busca pela origem
    fila.enfileirar(origem);
    visitado[origem] = true;

    // Laço principal do BFS
    while (!fila.estaVazia()) {
        int u = fila.desenfileirar();
        if (u == destino) break; // Encontrou o destino, pode parar a busca.

        for (int v = 0; v < numArmazens; ++v) {
            // Se 'v' é um vizinho de 'u' e ainda não foi visitado...
            if (matrizAdj[u][v] == 1 && !visitado[v]) {
                visitado[v] = true;
                predecessor[v] = u; // Guarda o caminho de volta
                fila.enfileirar(v);
            }
        }
    }

    Rota rota = {nullptr, 0};
    // Se o destino foi alcançado (tem um predecessor)
    if (predecessor[destino] != -1) {
        // Reconstrói o caminho de trás para frente, primeiro contando os passos
        int cont = 0;
        int atual = destino;
        while (predecessor[atual] != -1) {
            cont++;
            atual = predecessor[atual];
        }
        
        rota.numPassos = cont;
        rota.passos = new int[cont];
        
        // Preenche o array da rota na ordem correta
        atual = destino;
        for (int i = cont - 1; i >= 0; i--) {
            rota.passos[i] = atual;
            atual = predecessor[atual];
        }
    }

    // Libera a memória auxiliar
    delete[] visitado;
    delete[] predecessor;

    return rota;
}