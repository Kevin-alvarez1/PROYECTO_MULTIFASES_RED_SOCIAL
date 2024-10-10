#ifndef ARBOLABB_H
#define ARBOLABB_H

#include <string>
#include <list>
#include "Publicacion.h"
#include <vector>
#include <sstream>
#include <iomanip>

class NodoListaPublicacion {
public:
    Publicacion publicacion;
    NodoListaPublicacion* siguiente;

    NodoListaPublicacion(const Publicacion& pub) : publicacion(pub), siguiente(nullptr) {}
};

class ListaPublicaciones {
private:


public:
    NodoListaPublicacion* cabeza;
    NodoListaPublicacion* cola;

    ListaPublicaciones() : cabeza(nullptr), cola(nullptr) {}

    ~ListaPublicaciones() {
        while (cabeza) {
            NodoListaPublicacion* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
    }

    void agregarPublicacion(const Publicacion& publicacion) {
        NodoListaPublicacion* nuevoNodo = new NodoListaPublicacion(publicacion);
        if (!cabeza) {
            cabeza = nuevoNodo;
            cola = nuevoNodo;
        } else {
            cola->siguiente = nuevoNodo;
            cola = nuevoNodo;
        }
    }

    void copiarDesde(const ListaPublicaciones& otrasPublicaciones) {
        NodoListaPublicacion* actual = otrasPublicaciones.cabeza;
        while (actual) {
            agregarPublicacion(actual->publicacion); // Asumiendo que tienes un método para acceder a la publicación
            actual = actual->siguiente;
        }
    }

    void push_back(const Publicacion& publicacion) {
        NodoListaPublicacion* nuevoNodo = new NodoListaPublicacion(publicacion);
        if (!cabeza) {
            cabeza = nuevoNodo;
            cola = nuevoNodo;
        } else {
            cola->siguiente = nuevoNodo;
            cola = nuevoNodo;
        }
    }


    // Método para obtener el tamaño
    size_t size() const {
        size_t contador = 0;
        NodoListaPublicacion* actual = cabeza;
        while (actual) {
            contador++;
            actual = actual->siguiente;
        }
        return contador;
    }

    // Método para acceder a un elemento por índice
    Publicacion& operator[](size_t index) {
        if (index >= size()) {
            throw std::out_of_range("Índice fuera de rango"); // Manejo de errores si el índice es inválido
        }
        NodoListaPublicacion* actual = cabeza;
        for (size_t i = 0; i < index; ++i) {
            actual = actual->siguiente;
        }
        return actual->publicacion; // Asumiendo que NodoListaPublicacion tiene un atributo 'publicacion'
    }


};


class NodoABB {
public:
    std::string fecha;
    ListaPublicaciones publicaciones;
    NodoABB* izquierda;
    NodoABB* derecha;
    int altura;
    int id;
    NodoABB(int id_, const std::string& fecha_);
};

class ArbolABB {
public:
    ArbolABB();
    ~ArbolABB();

    void eliminarPublicacion(int id);
    void insertarPublicacion(const Publicacion& publicacion);
    void mostrarPublicaciones(const std::string& fecha) const;
    void mostrarPublicacionesCronologicas() const;
    ListaPublicaciones obtenerPublicacionesEnOrden(const std::string& tipoOrden) const;
    void generateDotFile(const std::string& filename, const std::string& fechaBuscada) const;
    void generateDot(NodoABB* nodo, std::ofstream& file, const std::string& fechaBuscada) const;
    void graficar(const std::string& archivoImagen) const;
    void preOrder(NodoABB* nodo, ListaPublicaciones& publicaciones) const;
    void inOrder(NodoABB* nodo, ListaPublicaciones& publicaciones) const;
    void postOrder(NodoABB* nodo, ListaPublicaciones& publicaciones) const;
    std::string inOrder(NodoABB* nodo) const;
    void mostrarPublicacionesCronologicas(const std::string& orden) const;
    void recorrerPreOrder(ListaPublicaciones& publicaciones) const;
    void recorrerInOrder(ListaPublicaciones& publicaciones) const;
    void recorrerPostOrder(ListaPublicaciones& publicaciones) const;
    NodoABB* getRaiz() const {
        return raiz;
    }

private:
    NodoABB* raiz;
    NodoABB* insertarNodo(NodoABB* nodo, const Publicacion& publicacion);
    NodoABB* buscarNodo(NodoABB* nodo, const std::string& fecha) const;
    void destruirArbol(NodoABB* nodo);
    int obtenerAltura(NodoABB* nodo);
    int obtenerBalance(NodoABB* nodo);
    void actualizarAltura(NodoABB* nodo);
    NodoABB* rotarDerecha(NodoABB* y);
    NodoABB* rotarIzquierda(NodoABB* x);
    NodoABB* eliminarNodo(NodoABB* nodo, int id);
    NodoABB* buscarMinimo(NodoABB* nodo) const;
    std::string convertirFecha(const std::string& fechaStr) const;
    void generateDot(NodoABB* nodo, std::ofstream& file) const;
    void recorrerPreOrder(NodoABB* nodo, ListaPublicaciones& publicaciones) const;
    void recorrerInOrder(NodoABB* nodo, ListaPublicaciones& publicaciones) const;
    void recorrerPostOrder(NodoABB* nodo, ListaPublicaciones& publicaciones) const;
};

#endif // ARBOLABB_H
