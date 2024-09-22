#include "Publicacion.h"
#include "publicacion.h"

#include <iostream>
// Constructor
Publicacion::Publicacion(int id, const std::string& correo, const std::string& contenido, const std::string& fecha, const std::string& hora)
    : id_(id), correo_(correo), contenido_(contenido), fecha_(fecha), hora_(hora), cabezaComentario_(nullptr) {}

// Destructor
Publicacion::~Publicacion() {

}

// Métodos Getters
int Publicacion::getId() const {
    return id_;
}

std::string Publicacion::getCorreo() const {
    return correo_;
}

std::string Publicacion::getContenido() const {
    return contenido_;
}

std::string Publicacion::getFecha() const {
    return fecha_;
}

std::string Publicacion::getHora() const {
    return hora_;
}

// Método Setter
void Publicacion::setContenido(const std::string& nuevoContenido) {
    contenido_ = nuevoContenido;
}

// Método para agregar un comentario
void Publicacion::agregarComentario(const Comentario& comentario) {
    NodoComentario* nuevoNodo = new NodoComentario(comentario);
    if (cabezaComentario_ == nullptr) {
        cabezaComentario_ = nuevoNodo;
    } else {
        NodoComentario* actual = cabezaComentario_;
        while (actual->siguiente != nullptr) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevoNodo;
    }
}

// Método para mostrar todos los comentarios
void Publicacion::mostrarComentarios() const {
    NodoComentario* actual = cabezaComentario_;
    if (actual == nullptr) {
        std::cout << "No hay comentarios para esta publicación." << std::endl;
        return;
    }

    std::cout << "Comentarios para la publicación ID " << id_ << ":" << std::endl;

    while (actual != nullptr) {
        const Comentario& comentario = actual->comentario;

        // Verifica que el comentario esté correctamente inicializado
        if (comentario.getCorreo().empty() || comentario.getFecha().empty() ||
            comentario.getHora().empty() || comentario.getComentario().empty()) {
            std::cerr << "Error: Datos del comentario no están completamente inicializados." << std::endl;
            return;
        }

        std::cout << "- Comentario de " << comentario.getCorreo()
                  << " (" << comentario.getFecha() << " a las " << comentario.getHora() << "): "
                  << comentario.getComentario() << std::endl;

        actual = actual->siguiente;
    }
}

// Método para limpiar comentarios
void Publicacion::limpiarComentarios() {
    NodoComentario* actual = cabezaComentario_;
    while (actual != nullptr) {
        NodoComentario* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
    cabezaComentario_ = nullptr;
}
