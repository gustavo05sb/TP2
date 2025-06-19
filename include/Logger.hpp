#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <string>
class Logger {
public:
    static void armazenado(double tempo, int idPacote, int idArmazem, int idSecao);
    static void removido(double tempo, int idPacote, int idArmazem, int idSecao);
    static void rearmazenado(double tempo, int idPacote, int idArmazem, int idSecao);
    static void emTransito(double tempo, int idPacote, int idOrigem, int idDestino);
    static void entregue(double tempo, int idPacote, int idArmazemFinal);
private:
    static std::string formatar(int valor, int largura);
};
#endif