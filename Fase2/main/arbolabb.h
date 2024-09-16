#ifndef ARBOLABB_H
#define ARBOLABB_H

#include <string>
#include <list>
#include "Publicacion.h"
#include <vector>

struct NodoABB {
    std::string fecha;
    std::list<Publicacion> publicaciones;
    NodoABB* izquierda;
    NodoABB* derecha;

    NodoABB(const std::string& fecha_);
    NodoABB* insertar(NodoABB* nodo, const Publicacion& publicacion);
    void mostrarEnOrden(NodoABB* nodo, const std::vector<std::string>& correosPermitidos) const;

};

class ArbolABB {
public:
    ArbolABB();
    ~ArbolABB();

    void insertarPublicacion(const Publicacion& publicacion);
    void mostrarPublicaciones(const std::string& fecha) const;
    void eliminarPublicaciones(const std::string& fecha);
    void mostrarPublicacionesCronologicas(const std::vector<std::string>& correosPermitidos) const;

private:
    NodoABB* raiz;

    NodoABB* insertarNodo(NodoABB* nodo, const Publicacion& publicacion);
    NodoABB* buscarNodo(NodoABB* nodo, const std::string& fecha) const;
    NodoABB* eliminarNodo(NodoABB* nodo, const std::string& fecha);
    void mostrarPublicacionesNodo(NodoABB* nodo, const std::string& fecha) const;
    void destruirArbol(NodoABB* nodo);
};

#endif // ARBOLABB_H
