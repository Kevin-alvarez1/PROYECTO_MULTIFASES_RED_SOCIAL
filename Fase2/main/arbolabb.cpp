#include "arbolabb.h"
#include <iostream>
#include <algorithm>
ArbolABB arbolABB;

// Constructor
ArbolABB::ArbolABB() : raiz(nullptr) {}

// Destructor
ArbolABB::~ArbolABB() {
    destruirArbol(raiz);
}

// Método para insertar una publicación
void ArbolABB::insertarPublicacion(const Publicacion& publicacion) {
    raiz = insertarNodo(raiz, publicacion);
}

// Método para mostrar publicaciones en una fecha específica
void ArbolABB::mostrarPublicaciones(const std::string& fecha) const {
    NodoABB* nodo = buscarNodo(raiz, fecha);
    if (nodo) {
        for (const auto& pub : nodo->publicaciones) {
            std::cout << "Contenido: " << pub.getContenido() << ", Fecha: " << pub.getFecha() << std::endl;
        }
    } else {
        std::cout << "No hay publicaciones para la fecha " << fecha << std::endl;
    }
}

// Método para eliminar publicaciones en una fecha específica
void ArbolABB::eliminarPublicaciones(const std::string& fecha) {
    raiz = eliminarNodo(raiz, fecha);
}

void NodoABB::mostrarEnOrden(NodoABB* nodo, const std::vector<std::string>& correosPermitidos) const {
    if (nodo == nullptr) {
        return;
    }

    // Recorrido en orden para mostrar las publicaciones en orden cronológico
    mostrarEnOrden(nodo->izquierda, correosPermitidos);

    // Filtrar las publicaciones que son del usuario o de sus amigos
    for (const auto& publicacion : nodo->publicaciones) {
        // Solo mostrar las publicaciones si el correo está en correosPermitidos
        if (std::find(correosPermitidos.begin(), correosPermitidos.end(), publicacion.getCorreo()) != correosPermitidos.end()) {
            std::cout << "Contenido: " << publicacion.getContenido()
            << ", Fecha: " << publicacion.getFecha()
            << ", Correo: " << publicacion.getCorreo() << std::endl;
        }
    }

    mostrarEnOrden(nodo->derecha, correosPermitidos);
}

void ArbolABB::mostrarPublicacionesCronologicas(const std::vector<std::string>& correosPermitidos) const {
    if (raiz) {
        raiz->mostrarEnOrden(raiz, correosPermitidos);
    }
}


// Método privado para insertar un nodo en el árbol
NodoABB* ArbolABB::insertarNodo(NodoABB* nodo, const Publicacion& publicacion) {
    if (!nodo) {
        return new NodoABB(publicacion.getFecha());
    }

    if (publicacion.getFecha() < nodo->fecha) {
        nodo->izquierda = insertarNodo(nodo->izquierda, publicacion);
    } else if (publicacion.getFecha() > nodo->fecha) {
        nodo->derecha = insertarNodo(nodo->derecha, publicacion);
    } else {
        // Fecha igual, agregar a la lista de publicaciones
        nodo->publicaciones.push_back(publicacion);
    }
    return nodo;
}

// Método privado para buscar un nodo por fecha
NodoABB* ArbolABB::buscarNodo(NodoABB* nodo, const std::string& fecha) const {
    if (!nodo || nodo->fecha == fecha) {
        return nodo;
    }

    if (fecha < nodo->fecha) {
        return buscarNodo(nodo->izquierda, fecha);
    } else {
        return buscarNodo(nodo->derecha, fecha);
    }
}

// Método privado para eliminar un nodo (y las publicaciones) por fecha
NodoABB* ArbolABB::eliminarNodo(NodoABB* nodo, const std::string& fecha) {
    if (!nodo) {
        return nullptr;
    }

    if (fecha < nodo->fecha) {
        nodo->izquierda = eliminarNodo(nodo->izquierda, fecha);
    } else if (fecha > nodo->fecha) {
        nodo->derecha = eliminarNodo(nodo->derecha, fecha);
    } else {
        // Nodo a eliminar encontrado
        if (!nodo->izquierda) {
            NodoABB* temp = nodo->derecha;
            delete nodo;
            return temp;
        } else if (!nodo->derecha) {
            NodoABB* temp = nodo->izquierda;
            delete nodo;
            return temp;
        }

        // Nodo con dos hijos: encontrar el nodo más pequeño en el subárbol derecho
        NodoABB* temp = nodo->derecha;
        while (temp && temp->izquierda) {
            temp = temp->izquierda;
        }

        nodo->fecha = temp->fecha;
        nodo->publicaciones = temp->publicaciones;
        nodo->derecha = eliminarNodo(nodo->derecha, temp->fecha);
    }
    return nodo;
}

// Método privado para mostrar publicaciones de un nodo específico
void ArbolABB::mostrarPublicacionesNodo(NodoABB* nodo, const std::string& fecha) const {
    if (nodo) {
        if (fecha == nodo->fecha) {
            for (const auto& pub : nodo->publicaciones) {
                std::cout << "Contenido: " << pub.getContenido() << ", Fecha: " << pub.getFecha() << std::endl;
            }
        } else if (fecha < nodo->fecha) {
            mostrarPublicacionesNodo(nodo->izquierda, fecha);
        } else {
            mostrarPublicacionesNodo(nodo->derecha, fecha);
        }
    }
}


// Método privado para destruir el árbol
void ArbolABB::destruirArbol(NodoABB* nodo) {
    if (nodo) {
        destruirArbol(nodo->izquierda);
        destruirArbol(nodo->derecha);
        delete nodo;
    }
}

// Constructor del NodoABB
NodoABB::NodoABB(const std::string& fecha_)
    : fecha(fecha_), izquierda(nullptr), derecha(nullptr) {}
