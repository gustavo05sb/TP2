#ifndef PACOTE_HPP
#define PACOTE_HPP

#include <iostream>
#include "Pathfinder.hpp" // Usa a struct Rota

/**
 * @class Pacote
 * @brief Representa um único pacote no sistema de transporte.
 *
 * Contém todas as informações pertinentes a um pacote, incluindo sua origem,
 * destino, a rota calculada que deve seguir e seu estado atual.
 */
class Pacote {
private:
    int id;
    int idArmazemOrigem;
    int idArmazemDestinoFinal;
    double tempoDePostagem;
    int idArmazemAtual; // Onde o pacote está fisicamente no momento.

    Rota rota; // A sequência de armazéns a visitar.
    int etapaAtualDaRota; // Índice para o array 'rota.passos'.

public:
    // Construtor principal.
    Pacote(int id, int origem, int destino, double tempoPostagem, Rota rotaCalculada);
    
    // Construtor de cópia e operador de atribuição são necessários
    // para gerenciar corretamente a memória do array da rota.
    Pacote(const Pacote& outro);
    Pacote& operator=(const Pacote& outro);
    
    // Destrutor para liberar o array da rota.
    ~Pacote();

    // Getters para os atributos do pacote.
    int getId() const { return id; }
    int getIdArmazemAtual() const { return idArmazemAtual; }
    double getTempoPostagem() const { return tempoDePostagem; }
    int getProximoDestino() const;

    // Setters e métodos de estado.
    void setIdArmazemAtual(int id) { idArmazemAtual = id; }
    void avancarEtapaRota();
    bool chegouAoDestinoFinal() const;
};

#endif // PACOTE_HPP