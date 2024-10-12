#ifndef ARBOLABB_H
#define ARBOLABB_H

#include <string>
#include <list>
#include "Publicacion.h"
#include <sstream>
#include <iomanip>

class NodoPublicacion {
public:
    Publicacion publicacion; // El objeto de Publicacion
    NodoPublicacion* siguiente; // Puntero al siguiente nodo

    NodoPublicacion(const Publicacion& pub);
};

class ListaPublicaciones {
public:
    NodoPublicacion* cabeza; // Puntero al primer nodo de la lista

    ListaPublicaciones() : cabeza(nullptr) {}

    void agregar(const Publicacion& pub) {
        NodoPublicacion* nuevoNodo = new NodoPublicacion(pub);
        nuevoNodo->siguiente = cabeza; // Inserta al principio
        cabeza = nuevoNodo;
    }
    int contar() const {
        int contador = 0;
        NodoPublicacion* actual = cabeza;
        while (actual != nullptr) {
            ++contador;
            actual = actual->siguiente;
        }
        return contador;
    }
    // Otros métodos para manejar la lista
};

class NodoFecha {
public:
    std::string fecha;
    NodoFecha* siguiente;

    NodoFecha(const std::string& fecha_) : fecha(fecha_), siguiente(nullptr) {}
};

class ListaFechas {
public:
    NodoFecha* cabeza;

    ListaFechas() : cabeza(nullptr) {}

    void agregarFecha(const std::string& fecha) {
        NodoFecha* nuevoNodo = new NodoFecha(fecha);
        nuevoNodo->siguiente = cabeza; // Inserta al principio
        cabeza = nuevoNodo;
    }

    void mostrarFechas() {
        NodoFecha* actual = cabeza;
        while (actual != nullptr) {
            std::cout << actual->fecha << std::endl; // Mostrar cada fecha
            actual = actual->siguiente;
        }
    }

    ~ListaFechas() {
        // Destructor para liberar memoria
        while (cabeza != nullptr) {
            NodoFecha* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
    }
};

class NodoABB {
public:
    std::string fecha;
    NodoPublicacion* publicaciones; // Cambiado a un puntero a NodoPublicacion
    NodoABB* izquierda;
    NodoABB* derecha;
    int altura;
    int id;

    // Constructor
    NodoABB(int id_, const std::string& fecha_);
    ~NodoABB();
};
class ArbolABB {
public:
    ArbolABB();
    ~ArbolABB();

    void eliminarPublicacion(int id);
    void insertarPublicacion(const Publicacion& publicacion);
    void mostrarPublicaciones(const std::string& fecha) const;
    void mostrarPublicacionesCronologicas() const;
    std::vector<Publicacion> obtenerPublicacionesEnOrden(const std::string& tipoOrden) const;
    void generateDotFile(const std::string& filename, const std::string& fechaBuscada) const;
    void generateDot(NodoABB* nodo, std::ofstream& file, const std::string& fechaBuscada) const;
    void graficar(const std::string& archivoImagen) const;
    std::string inOrder(NodoABB* nodo) const;
    void mostrarPublicacionesCronologicas(const std::string& orden) const;
    void recorrerPreOrder(NodoABB* nodo, Publicacion**& publicaciones, int& cantidad, int& capacidad) const;
    void recorrerInOrder(NodoABB* nodo, Publicacion**& publicaciones, int& cantidad, int& capacidad) const;
    void recorrerPostOrder(NodoABB* nodo, Publicacion**& publicaciones, int& cantidad, int& capacidad) const;
    void liberarPublicaciones(Publicacion** publicaciones, int cantidad) const; // Liberar la memoria de las publicaciones
    void recorrerPreOrder(Publicacion**& publicaciones, int& cantidad, int& capacidad) const;
    void recorrerInOrder(Publicacion**& publicaciones, int& cantidad, int& capacidad) const;
    void recorrerPostOrder(Publicacion**& publicaciones, int& cantidad, int& capacidad) const;

    NodoABB* getRaiz() const {
        return raiz;
    }
    int size() {
        return contarNodos(raiz);
    }
    void obtenerPublicacionesPorFecha(const std::string& fecha, ListaPublicaciones& publicaciones) {
        obtenerPublicacionesPorFechaRecursivo(raiz, fecha, publicaciones);
    }
    void obtenerTodasLasPublicaciones(ListaPublicaciones& publicaciones) {
        obtenerTodasLasPublicacionesRecursivo(raiz, publicaciones);
    }
    void recorrerInOrder(NodoABB* nodo, ListaFechas& fechas) const;


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

    void agregarPublicacion(Publicacion**& publicaciones, int& cantidad, int& capacidad, const Publicacion& publicacion) const;
    int contarNodos(NodoABB* nodo) {
        if (nodo == nullptr) return 0;
        return 1 + contarNodos(nodo->izquierda) + contarNodos(nodo->derecha);
    }
    void obtenerPublicacionesPorFechaRecursivo(NodoABB* nodo, const std::string& fecha, ListaPublicaciones& publicaciones) {
        if (nodo != nullptr) {
            // Llamada recursiva en el subárbol izquierdo
            obtenerPublicacionesPorFechaRecursivo(nodo->izquierda, fecha, publicaciones);

            // Verificar las publicaciones en el nodo actual
            NodoPublicacion* actual = nodo->publicaciones; // Suponiendo que nodo->publicaciones es el primer nodo de la lista enlazada
            while (actual != nullptr) {
                if (actual->publicacion.getFecha() == fecha) {
                    publicaciones.agregar(actual->publicacion); // Agregar la publicación si coincide con la fecha
                }
                actual = actual->siguiente; // Mover al siguiente nodo de la lista
            }

            // Llamada recursiva en el subárbol derecho
            obtenerPublicacionesPorFechaRecursivo(nodo->derecha, fecha, publicaciones);
        }
    }
    void obtenerTodasLasPublicacionesRecursivo(NodoABB* nodo, ListaPublicaciones& publicaciones) {
        if (nodo != nullptr) {
            // Llamada recursiva al subárbol izquierdo
            obtenerTodasLasPublicacionesRecursivo(nodo->izquierda, publicaciones);

            // Verificar las publicaciones en el nodo actual
            NodoPublicacion* actual = nodo->publicaciones; // Suponiendo que nodo->publicaciones es el primer nodo de la lista enlazada
            while (actual != nullptr) {
                publicaciones.agregar(actual->publicacion); // Agregar la publicación a la lista
                actual = actual->siguiente; // Mover al siguiente nodo de la lista
            }

            // Llamada recursiva al subárbol derecho
            obtenerTodasLasPublicacionesRecursivo(nodo->derecha, publicaciones);
        }
    }

};

#endif // ARBOLABB_H
