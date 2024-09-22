#ifndef PUBLICACION_H
#define PUBLICACION_H

#include <string>
#include "comentario.h"
#include "comentarios.h"

class NodoComentario {
public:
    NodoComentario(const Comentario& com) : comentario(com), siguiente(nullptr) {}

    Comentario comentario;
    NodoComentario* siguiente;

private:
    NodoComentario() = delete;
    NodoComentario(const NodoComentario&) = delete;
    NodoComentario& operator=(const NodoComentario&) = delete;
};


class Publicacion {
public:
    Publicacion(int id, const std::string& correo, const std::string& contenido, const std::string& fecha, const std::string& hora);

    ~Publicacion();

    int getId() const;
    std::string getCorreo() const;
    std::string getContenido() const;
    std::string getFecha() const;
    std::string getHora() const;

    void setContenido(const std::string& nuevoContenido);

    void agregarComentario(const Comentario& comentario);
    void mostrarComentarios() const;
    void limpiarComentarios();
    std::string toString() const {
        return "Congenitod: " + getContenido() + "Contenido: " + getContenido() + ", Fecha: " + getFecha() + ", Hora: " + getHora();
    }

private:
    int id_;
    std::string correo_;
    std::string contenido_;
    std::string fecha_;
    std::string hora_;
    NodoComentario* cabezaComentario_;

};

#endif // PUBLICACION_H
