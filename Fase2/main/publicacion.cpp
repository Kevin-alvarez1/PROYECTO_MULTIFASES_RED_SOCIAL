#include "Publicacion.h"
#include <iostream>
#include "publicacion.h"


Publicacion::Publicacion(int id, const std::string &correo, const std::string &contenido, const std::string &fecha, const std::string &hora)
    : id_(id), correo_(correo), contenido_(contenido), fecha_(fecha), hora_(hora) {}

int Publicacion::getId() const { return id_; }
std::string Publicacion::getCorreo() const { return correo_; }
std::string Publicacion::getContenido() const { return contenido_; }
std::string Publicacion::getFecha() const { return fecha_; }
std::string Publicacion::getHora() const { return hora_; }
std::vector<Comentario> Publicacion::getComentarios() const { return comentarios_; }

void Publicacion::setId(int id) { id_ = id; }
void Publicacion::setCorreo(const std::string &correo) { correo_ = correo; }
void Publicacion::setContenido(const std::string &contenido) { contenido_ = contenido; }
void Publicacion::setFecha(const std::string &fecha) { fecha_ = fecha; }
void Publicacion::setHora(const std::string &hora) { hora_ = hora; }
void Publicacion::agregarComentario(const Comentario &comentario) { comentarios_.push_back(comentario); }
void Publicacion::mostrarComentarios() const {
    for (const auto &comentario : comentarios_) {
        std::cout << "Comentario por " << comentario.getCorreo() << ": " << comentario.getComentario() << std::endl;
    }
}


void Publicacion::mostrarInformacion() const {
    std::cout << "ID: " << id_ << std::endl;
    std::cout << "Correo: " << correo_ << std::endl;
    std::cout << "Contenido: " << contenido_ << std::endl;
    std::cout << "Fecha: " << fecha_ << std::endl;
    std::cout << "Hora: " << hora_ << std::endl;
    mostrarComentarios();
}
