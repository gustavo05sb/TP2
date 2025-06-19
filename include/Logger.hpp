#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

/**
 * @class Logger
 * @brief Centraliza e padroniza toda a impressão de logs da simulação.
 *
 * Usar uma classe estática para logging garante que todas as mensagens sigam
 * o mesmo formato (zeros à esquerda, texto padrão), facilitando a depuração e análise.
 */
class Logger {
public:
    static void armazenado(double tempo, int idPacote, int idArmazem, int idSecao);
    static void removido(double tempo, int idPacote, int idArmazem, int idSecao);
    static void rearmazenado(double tempo, int idPacote, int idArmazem, int idSecao);
    static void emTransito(double tempo, int idPacote, int idOrigem, int idDestino);
    static void entregue(double tempo, int idPacote, int idArmazemFinal);

private:
    // Função helper para formatar números com uma largura fixa e zeros à esquerda.
    static std::string formatar(int valor, int largura);
};

#endif // LOGGER_HPP