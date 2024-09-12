#include "Comentario.h"

// Constructor
Comentario::Comentario(const std::string& correo, const std::string& comentario, const std::string& fecha, const std::string& hora)
    : correo_(correo), comentario_(comentario), fecha_(fecha), hora_(hora) {}

// Getters
std::string Comentario::getCorreo() const {
    return correo_;
}

std::string Comentario::getComentario() const {
    return comentario_;
}

std::string Comentario::getFecha() const {
    return fecha_;
}

std::string Comentario::getHora() const {
    return hora_;
}
