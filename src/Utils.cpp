#include "Utils.hpp"

void Utils::ordenarPacotesPorId(Pacote** pacotes, int tamanho) {
    if (tamanho < 2) return;

    bool trocou;
    do {
        trocou = false;
        for (int i = 0; i < tamanho - 1; ++i) {
            if (pacotes[i]->getId() > pacotes[i + 1]->getId()) {
                // Troca os ponteiros no array
                Pacote* temp = pacotes[i];
                pacotes[i] = pacotes[i + 1];
                pacotes[i + 1] = temp;
                trocou = true;
            }
        }
    } while (trocou);
}