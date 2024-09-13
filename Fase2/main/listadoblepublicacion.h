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

    NodoPublicacion *cabeza;  // Asegúrate de usar este nombre en el .cpp
    NodoPublicacion *cola;    // Asegúrate de usar este nombre en el .cpp
    int siguienteId;

    void crearPNG(const std::string &dotFilename, const std::string &pngFilename);

public:
    ListaDoblePublicacion();
    ~ListaDoblePublicacion();
    void generateDot(const std::string &filename);
    void cargarPublicacionesDesdeJson(const std::string &filename);
    void agregarPublicacion(const Publicacion &publicacion);
    void mostrarTodasLasPublicaciones() const;
};

#endif // LISTADOBLEPUBLICACION_H
