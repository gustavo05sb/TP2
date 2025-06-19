#ifndef EVENTO_HPP
#define EVENTO_HPP
class Simulador;
enum TipoEvento { CHEGADA_PACOTE, TRANSPORTE_DIARIO };
class Evento {
protected:
    double tempo;
    const long long id;
private:
    static long long proximoId;
public:
    Evento(double tempo);
    virtual ~Evento() {}
    double getTempo() const;
    long long getId() const;
    virtual void processar(Simulador* sim) = 0;
    virtual TipoEvento getTipo() const = 0;
};
#endif