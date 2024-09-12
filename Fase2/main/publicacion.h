#ifndef PUBLICACION_H
#define PUBLICACION_H

#include <string>
#include "Comentario.h"

class Publicacion {
public:
    // Constructor
    Publicacion(int id, const std::string& correo, const std::string& contenido, const std::string& fecha, const std::string& hora);

    // Destructor para limpiar la memoria
    ~Publicacion();

    // Getters
    int getId() const;
    std::string getCorreo() const;
    std::string getContenido() const;
    std::string getFecha() const;
    std::string getHora() const;

    // Setters
    void setContenido(const std::string& nuevoContenido);

    // Otros métodos
    void agregarComentario(const Comentario& comentario);
    void mostrarComentarios() const;
    void limpiarComentarios(); // Método para limpiar la lista de comentarios al destruir la publicación

private:
    int id_;
    std::string correo_;
    std::string contenido_;
    std::string fecha_;
    std::string hora_;

    // Estructura de nodo para manejar la lista enlazada de comentarios
    class NodoComentario {
    public:
        // Constructor
        NodoComentario(const Comentario& com) : comentario(com), siguiente(nullptr) {}

        // Miembros accesibles para la clase Publicacion
        Comentario comentario;
        NodoComentario* siguiente;

    private:
        // Constructor por defecto y copia privados para evitar uso no deseado
        NodoComentario() = delete;
        NodoComentario(const NodoComentario&) = delete;
        NodoComentario& operator=(const NodoComentario&) = delete;
    };

    NodoComentario* cabezaComentario_; // Puntero al primer nodo de la lista de comentarios

};

#endif // PUBLICACION_H
