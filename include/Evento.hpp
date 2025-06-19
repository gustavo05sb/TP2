#ifndef EVENTO_HPP
#define EVENTO_HPP

class Simulador; 

// Enum para identificar os tipos de evento de forma clara.
enum TipoEvento {
    CHEGADA_PACOTE,
    TRANSPORTE_DIARIO
};

/**
 * @class Evento
 * @brief Classe base abstrata para todos os eventos da simulação.
 *
 * Define a interface comum para um evento: ele tem um tempo de ocorrência e uma
 * lógica de processamento. Usamos polimorfismo (funções virtuais) para que o
 * simulador possa tratar todos os eventos da mesma forma.
 */
class Evento {
protected:
    double tempo;       // Tempo em que o evento ocorre.
    const long long id; // ID único e sequencial para desempate (determinismo).

private:
    static long long proximoId; // Contador estático para gerar IDs únicos.

public:
    // Construtor: atribui o tempo e um ID único.
    Evento(double tempo) : tempo(tempo), id(proximoId++) {}
    
    // Destrutor virtual é essencial para classes base com funções virtuais.
    virtual ~Evento() {}

    double getTempo() const { return tempo; }
    long long getId() const { return id; }

    // Método virtual puro: cada classe de evento derivada DEVE implementar sua própria lógica.
    virtual void processar(Simulador* sim) = 0;
    virtual TipoEvento getTipo() const = 0;
};

#endif // EVENTO_HPP