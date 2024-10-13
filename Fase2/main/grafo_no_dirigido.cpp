#include "grafo_no_dirigido.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>

GrafoNoDirigido grafoNoDirigido;


const int INICIAL_CAPACIDAD = 5;

// Constructor de Nodo
Nodo::Nodo(const std::string& nombre) : nombre(nombre), numVecinos(0), capacidadVecinos(INICIAL_CAPACIDAD) {
    vecinos = new Nodo*[capacidadVecinos];  // Crear arreglo dinámico para vecinos
}

// Destructor de Nodo
Nodo::~Nodo() {
    delete[] vecinos;  // Liberar memoria de los vecinos
}

// Agregar vecino, redimensionando si es necesario
void Nodo::agregarVecino(Nodo* vecino) {
    if (numVecinos == capacidadVecinos) {
        capacidadVecinos *= 2;
        Nodo** nuevoArreglo = new Nodo*[capacidadVecinos];
        std::memcpy(nuevoArreglo, vecinos, numVecinos * sizeof(Nodo*));
        delete[] vecinos;
        vecinos = nuevoArreglo;
    }
    vecinos[numVecinos++] = vecino;
}

// Verificar si el nodo ya tiene un vecino específico
bool Nodo::tieneVecino(Nodo* nodo) const {
    for (int i = 0; i < numVecinos; i++) {
        if (vecinos[i] == nodo) {
            return true;
        }
    }
    return false;
}

// Constructor de GrafoNoDirigido
GrafoNoDirigido::GrafoNoDirigido() : numNodos(0), capacidadNodos(INICIAL_CAPACIDAD) {
    nodos = new Nodo*[capacidadNodos];
}

// Destructor de GrafoNoDirigido
GrafoNoDirigido::~GrafoNoDirigido() {
    for (int i = 0; i < numNodos; i++) {
        delete nodos[i];  // Liberar la memoria de cada nodo
    }
    delete[] nodos;  // Liberar el arreglo dinámico de nodos
}

// Redimensionar el arreglo de nodos cuando es necesario
void GrafoNoDirigido::redimensionarNodos() {
    capacidadNodos *= 2;
    Nodo** nuevoArreglo = new Nodo*[capacidadNodos];
    std::memcpy(nuevoArreglo, nodos, numNodos * sizeof(Nodo*));
    delete[] nodos;
    nodos = nuevoArreglo;
}

// Buscar un nodo por nombre
Nodo* GrafoNoDirigido::encontrarNodo(const std::string& nombre) const {
    for (int i = 0; i < numNodos; i++) {
        if (nodos[i]->nombre == nombre) {
            return nodos[i];
        }
    }
    return nullptr;
}

// Verificar si un nodo ya existe en el grafo
bool GrafoNoDirigido::existeNombre(const std::string& nombre) const {
    return encontrarNodo(nombre) != nullptr;
}

// Insertar un nuevo nodo si no existe
void GrafoNoDirigido::insertarNombre(const std::string& nombre) {
    if (!existeNombre(nombre)) {
        if (numNodos == capacidadNodos) {
            redimensionarNodos();
        }
        nodos[numNodos++] = new Nodo(nombre);
    }
}

// Insertar una relación entre dos nodos
void GrafoNoDirigido::insertarRelacion(const std::string& nombre1, const std::string& nombre2) {
    Nodo* nodo1 = encontrarNodo(nombre1);
    Nodo* nodo2 = encontrarNodo(nombre2);

    if (!nodo1) {
        insertarNombre(nombre1);
        nodo1 = encontrarNodo(nombre1);
    }
    if (!nodo2) {
        insertarNombre(nombre2);
        nodo2 = encontrarNodo(nombre2);
    }

    if (!nodo1->tieneVecino(nodo2)) {
        nodo1->agregarVecino(nodo2);
        nodo2->agregarVecino(nodo1);  // Relación bidireccional
    }
}

// Obtener los amigos (vecinos) de un nodo
std::string* GrafoNoDirigido::obtenerAmigos(const std::string& nombre, int& cantidadAmigos) const {
    Nodo* nodo = encontrarNodo(nombre);
    if (!nodo) {
        cantidadAmigos = 0;
        return nullptr;
    }

    cantidadAmigos = nodo->numVecinos;
    std::string* amigos = new std::string[cantidadAmigos];
    for (int i = 0; i < cantidadAmigos; i++) {
        amigos[i] = nodo->vecinos[i]->nombre;
    }
    return amigos;
}

// Mostrar el grafo
void GrafoNoDirigido::mostrarGrafo() const {
    for (int i = 0; i < numNodos; i++) {
        std::cout << nodos[i]->nombre << ": ";
        for (int j = 0; j < nodos[i]->numVecinos; j++) {
            std::cout << nodos[i]->vecinos[j]->nombre << " ";
        }
        std::cout << std::endl;
    }
}

// Generar un archivo DOT para visualizar el grafo
void GrafoNoDirigido::generarArchivoDOT(const std::string& nombreArchivo) const {
    std::ofstream archivo(nombreArchivo);
    if (archivo.is_open()) {
        archivo << "graph G {\n";
        for (int i = 0; i < numNodos; i++) {
            for (int j = 0; j < nodos[i]->numVecinos; j++) {
                if (nodos[i]->nombre < nodos[i]->vecinos[j]->nombre) {  // Evitar duplicados
                    archivo << "    \"" << nodos[i]->nombre << "\" -- \"" << nodos[i]->vecinos[j]->nombre << "\";\n";
                }
            }
        }
        archivo << "}\n";
        archivo.close();
    }
}

// Generar un PNG del grafo usando Graphviz
void GrafoNoDirigido::generarPNG(const std::string& nombreArchivo) const {
    std::string dotFile = nombreArchivo + ".dot";
    generarArchivoDOT(dotFile);
    std::string comando = "dot -Tpng " + dotFile + " -o " + nombreArchivo + ".png";
    system(comando.c_str());
}
