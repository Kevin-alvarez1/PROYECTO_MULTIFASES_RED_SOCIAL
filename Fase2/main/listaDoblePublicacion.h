#ifndef LISTADOBLEPUBLICACION_H
#define LISTADOBLEPUBLICACION_H

#include "publicacion.h"
#include <string>
#include "matrizdispersa.h"
#include "arbolabb.h"
#include "arbolbcomentario.h"
#include "comentario.h"
extern MatrizDispersa matrizDispersa;
extern ArbolABB arbolABB;
extern ArbolBComentario arbolComentarios_;


class ListaDoblePublicacion
{
private:
    class NodoPublicacion
    {
    public:
        Publicacion publicacion;
        NodoPublicacion *siguiente;
        NodoPublicacion *anterior;

        NodoPublicacion(const Publicacion &publicacion);
    };

    NodoPublicacion *cabeza;
    NodoPublicacion *cola;
    int siguienteId;

    void crearPNG(const std::string &dotFilename, const std::string &pngFilename);
    NodoABB* raiz;


public:
    ListaDoblePublicacion();
    ~ListaDoblePublicacion();
    void generateDot(const std::string &filename);
    void cargarPublicacionesDesdeJson(const std::string &filename);
    void agregarPublicacion(const Publicacion &publicacion);
    void mostrarPublicacion(const Publicacion& publicacion) const;
    void mostrarPublicacionesPorUsuario(const std::string& correo) const;
    void mostrarTodasLasPublicaciones() const;
    void mostrarPublicacionesPorCorreo(const std::string& correo) const;
    ListaPublicaciones mostrarPublicacionesYAmigos(const std::string &correo, const MatrizDispersa &matriz, ArbolABB &arbol, const std::string &orden);
    int obtenerNuevoId() const;
    void mostrarPublicacionesOrden(const std::string& correoUsuario, const MatrizDispersa& matrizDispersa, ArbolABB& arbolABB, int orden) const;
    void obtenerPublicacionesEnOrden(const std::string& tipoOrden) const;
    void eliminarPublicacionPorId(int id);
};

#endif // LISTADOBLEPUBLICACION_H
