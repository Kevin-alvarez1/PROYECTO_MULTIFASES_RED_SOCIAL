#include "arbolabb.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <cstdlib>

ArbolABB arbolABB;
// Constructor
ArbolABB::ArbolABB() : raiz(nullptr) {}

// Destructor
ArbolABB::~ArbolABB() {
    destruirArbol(raiz);
}

NodoABB::NodoABB(const std::string& fecha_) : fecha(fecha_), izquierda(nullptr), derecha(nullptr) {
}

// Insertar una publicación en el ABB
void ArbolABB::insertarPublicacion(const Publicacion& publicacion) {
    raiz = insertarNodo(raiz, publicacion);
}

// Insertar un nodo basado en la fecha de la publicación
NodoABB* ArbolABB::insertarNodo(NodoABB* nodo, const Publicacion& publicacion) {
    std::string fechaConvertida = convertirFecha(publicacion.getFecha());

    if (!nodo) {
        // Si el nodo no existe, crear uno nuevo
        NodoABB* nuevoNodo = new NodoABB(fechaConvertida);
        nuevoNodo->publicaciones.push_back(publicacion);
        return nuevoNodo;
    }

    // Comparar fechas para determinar la posición
    if (fechaConvertida < nodo->fecha) {
        nodo->izquierda = insertarNodo(nodo->izquierda, publicacion);
    } else if (fechaConvertida > nodo->fecha) {
        nodo->derecha = insertarNodo(nodo->derecha, publicacion);
    } else {
        // Si la fecha es igual, añadir la publicación a la lista
        nodo->publicaciones.push_back(publicacion);
    }

    return nodo;
}

// Buscar un nodo por fecha
NodoABB* ArbolABB::buscarNodo(NodoABB* nodo, const std::string& fecha) const {
    std::string fechaConvertida = convertirFecha(fecha);

    if (!nodo || nodo->fecha == fechaConvertida) {
        return nodo;
    }

    if (fechaConvertida < nodo->fecha) {
        return buscarNodo(nodo->izquierda, fecha);
    } else {
        return buscarNodo(nodo->derecha, fecha);
    }
}

// Mostrar todas las publicaciones en orden cronológico
void ArbolABB::mostrarPublicacionesCronologicas() const {
    std::vector<Publicacion> publicaciones;
    inOrder(raiz, publicaciones);

    for (const auto& pub : publicaciones) {
        std::cout << "Fecha: " << pub.getFecha() << ", Contenido: " << pub.getContenido() << std::endl;
    }
}

std::string ArbolABB::inOrder(NodoABB* nodo) const {
    std::vector<Publicacion> publicaciones; // Vector para almacenar publicaciones
    inOrder(nodo, publicaciones); // Llamar al método existente

    // Construir el string a partir del vector
    std::ostringstream result;
    for (const auto& publicacion : publicaciones) {
        result << "ID: " << publicacion.getId()
        << ", Correo: " << publicacion.getCorreo()
        << ", Contenido: " << publicacion.getContenido() << " - ";
    }
    return result.str(); // Retornar el resultado como string
}

// Mantener el método existente
void ArbolABB::inOrder(NodoABB* nodo, std::vector<Publicacion>& publicaciones) const {
    if (nodo) {
        inOrder(nodo->izquierda, publicaciones);
        publicaciones.insert(publicaciones.end(), nodo->getPublicaciones().begin(), nodo->getPublicaciones().end());
        inOrder(nodo->derecha, publicaciones);
    }
}




void ArbolABB::generateDotFile(const std::string& filename) const {
    std::ofstream file(filename + ".dot");
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo DOT para escribir." << std::endl;
        return;
    }

    file << "digraph BSTTree {\n";
    file << "node [shape=circle];\n";

    generateDot(raiz, file);

    file << "}\n";
    file.close();

    // Convertir el archivo DOT a PNG usando el comando dot
    std::string comando = "dot -Tpng " + filename + ".dot -o " + filename + ".png";
    int resultado = std::system(comando.c_str());

    if (resultado != 0) {
        std::cerr << "Error al convertir el archivo DOT a PNG." << std::endl;
    }
}

void ArbolABB::generateDot(NodoABB* nodo, std::ofstream& file) const {
    if (nodo) {
        // Asegúrate de que cada nodo esté conectado a sus nodos hijos
        if (nodo->izquierda) {
            file << "\"" << nodo->fecha << "\" -> \"" << nodo->izquierda->fecha << "\";\n";
            generateDot(nodo->izquierda, file);
        }
        if (nodo->derecha) {
            file << "\"" << nodo->fecha << "\" -> \"" << nodo->derecha->fecha << "\";\n";
            generateDot(nodo->derecha, file);
        }
    }
}

// Convertir la fecha a un formato ordenado (YYYY/MM/DD)
std::string ArbolABB::convertirFecha(const std::string& fechaStr) const {
    int dia, mes, ano;
    char separador;
    std::istringstream iss(fechaStr);
    iss >> dia >> separador >> mes >> separador >> ano;

    std::ostringstream oss;
    oss << std::setw(4) << std::setfill('0') << ano << "/"
        << std::setw(2) << std::setfill('0') << mes << "/"
        << std::setw(2) << std::setfill('0') << dia;

    return oss.str();
}

// Destruir el árbol
void ArbolABB::destruirArbol(NodoABB* nodo) {
    if (nodo) {
        destruirArbol(nodo->izquierda);
        destruirArbol(nodo->derecha);
        delete nodo;
    }
}
