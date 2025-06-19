#include "Logger.hpp"
#include <iostream>
#include <iomanip> // Para std::setw, std::setfill
#include <sstream> // Para std::stringstream

// Implementação da função de formatação.
std::string Logger::formatar(int valor, int largura) {
    std::stringstream ss;
    ss << std::setw(largura) << std::setfill('0') << valor;
    return ss.str();
}

void Logger::armazenado(double tempo, int idPacote, int idArmazem, int idSecao) {
    std::cout << formatar(static_cast<int>(tempo), 7)
              << " pacote " << formatar(idPacote, 3)
              << " armazenado em " << formatar(idArmazem, 3)
              << " na secao " << formatar(idSecao, 3) << std::endl;
}

void Logger::removido(double tempo, int idPacote, int idArmazem, int idSecao) {
    std::cout << formatar(static_cast<int>(tempo), 7)
              << " pacote " << formatar(idPacote, 3)
              << " removido de " << formatar(idArmazem, 3)
              << " na secao " << formatar(idSecao, 3) << std::endl;
}

void Logger::rearmazenado(double tempo, int idPacote, int idArmazem, int idSecao) {
    std::cout << formatar(static_cast<int>(tempo), 7)
              << " pacote " << formatar(idPacote, 3)
              << " rearmazenado em " << formatar(idArmazem, 3)
              << " na secao " << formatar(idSecao, 3) << std::endl;
}

void Logger::emTransito(double tempo, int idPacote, int idOrigem, int idDestino) {
    std::cout << formatar(static_cast<int>(tempo), 7)
              << " pacote " << formatar(idPacote, 3)
              << " em transito de " << formatar(idOrigem, 3)
              << " para " << formatar(idDestino, 3) << std::endl;
}

void Logger::entregue(double tempo, int idPacote, int idArmazemFinal) {
    std::cout << formatar(static_cast<int>(tempo), 7)
              << " pacote " << formatar(idPacote, 3)
              << " entregue em " << formatar(idArmazemFinal, 3) << std::endl;
}