#ifndef ARBOLABB_H
#define ARBOLABB_H

#include <string>
#include <list>
#include "Publicacion.h"
#include <vector>
#include <sstream>
#include <iomanip>

struct NodoABB {
    std::string fecha;
    std::list<Publicacion> publicaciones;
    NodoABB* izquierda;
    NodoABB* derecha;

    NodoABB(const std::string& fecha_);
    NodoABB* insertar(NodoABB* nodo, const Publicacion& publicacion);
    void mostrarEnOrden(NodoABB* nodo) const;
    std::list<Publicacion> getPublicaciones() const { return publicaciones; }
    NodoABB* getIzquierda() const { return izquierda; }
    NodoABB* getDerecha() const { return derecha; }
};

class ArbolABB {
public:
    ArbolABB();
    ~ArbolABB();

    void insertarPublicacion(const Publicacion& publicacion);
    void mostrarPublicaciones(const std::string& fecha) const;
    void mostrarPublicacionesCronologicas() const;
    std::vector<Publicacion> obtenerPublicacionesEnOrden(const std::string& tipoOrden) const;
    void generateDotFile(const std::string& filename) const;
    void generateDotNode(NodoABB* nodo, std::ofstream& file) const;
    void graficar(const std::string& archivoImagen) const;

private:
    NodoABB* raiz;
    NodoABB* insertarNodo(NodoABB* nodo, const Publicacion& publicacion);
    NodoABB* buscarNodo(NodoABB* nodo, const std::string& fecha) const;
    void destruirArbol(NodoABB* nodo);
    void preOrder(NodoABB* nodo, std::vector<Publicacion>& publicaciones) const;
    void inOrder(NodoABB* nodo, std::vector<Publicacion>& publicaciones) const;
    void postOrder(NodoABB* nodo, std::vector<Publicacion>& publicaciones) const;
    std::string convertirFecha(const std::string& fechaStr) const;
    void generateDot(NodoABB* nodo, std::ofstream& file) const;

};

#endif // ARBOLABB_H
