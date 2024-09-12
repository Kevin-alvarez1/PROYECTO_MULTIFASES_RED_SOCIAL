#ifndef LISTADOBLEPUBLICACION_H
#define LISTADOBLEPUBLICACION_H

#include "publicacion.h"
#include <string>

class ListaDoblePublicacion
{
private:
    struct NodoPublicacion
    {
        Publicacion publicacion;
        NodoPublicacion *siguiente;
        NodoPublicacion *anterior;

        NodoPublicacion(const Publicacion &publicacion);
    };

    NodoPublicacion *cabeza;
    NodoPublicacion *cola;
    int siguienteId;

public:
    ListaDoblePublicacion();
    ~ListaDoblePublicacion();

    void cargarPublicacionesDesdeJson(const std::string &filename);
    void agregarPublicacion(const Publicacion &publicacion);
    void mostrarTodasLasPublicaciones() const;
};

#endif // LISTADOBLEPUBLICACION_H
