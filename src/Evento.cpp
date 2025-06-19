#include "Evento.hpp"
long long Evento::proximoId = 0;
Evento::Evento(double tempo) : tempo(tempo), id(proximoId++) {}
double Evento::getTempo() const { return tempo; }
long long Evento::getId() const { return id; }