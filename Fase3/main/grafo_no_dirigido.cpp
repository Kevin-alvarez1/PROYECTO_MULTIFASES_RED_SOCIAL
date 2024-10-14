#include "grafo_no_dirigido.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <algorithm>

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

std::string* GrafoNoDirigido::recomendarAmigos(const std::string& nombre, int& cantidadRecomendaciones) const {
    Nodo* nodo = encontrarNodo(nombre);
    if (!nodo) {
        cantidadRecomendaciones = 0;
        return nullptr; // Usuario no encontrado
    }

    // Mapa para contar cuántos amigos en común tiene cada sugerencia
    class Recomendacion {
    public:
        Nodo* nodo;
        int amigosEnComun;
    };

    Recomendacion* recomendaciones = new Recomendacion[capacidadNodos]; // Arreglo dinámico para almacenar sugerencias
    int numRecomendaciones = 0;

    // Obtener los amigos del usuario actual
    for (int i = 0; i < nodo->numVecinos; ++i) {
        Nodo* amigo = nodo->vecinos[i];

        // Obtener los amigos de los amigos (a dos saltos)
        for (int j = 0; j < amigo->numVecinos; ++j) {
            Nodo* amigoDeAmigo = amigo->vecinos[j];

            // Evitar recomendar al mismo usuario o sus amigos directos
            if (amigoDeAmigo == nodo || nodo->tieneVecino(amigoDeAmigo)) {
                continue;
            }

            // Verificar si ya hemos contado a este nodo como recomendación
            bool encontrado = false;
            for (int k = 0; k < numRecomendaciones; ++k) {
                if (recomendaciones[k].nodo == amigoDeAmigo) {
                    recomendaciones[k].amigosEnComun++; // Incrementar el número de amigos en común
                    encontrado = true;
                    break;
                }
            }

            // Si no está en las recomendaciones, agregarlo
            if (!encontrado) {
                recomendaciones[numRecomendaciones].nodo = amigoDeAmigo;
                recomendaciones[numRecomendaciones].amigosEnComun = 1;
                numRecomendaciones++;
            }
        }
    }

    // Ordenar las recomendaciones por la cantidad de amigos en común (de mayor a menor)
    std::sort(recomendaciones, recomendaciones + numRecomendaciones, [](const Recomendacion& a, const Recomendacion& b) {
        return a.amigosEnComun > b.amigosEnComun;
    });

    // Crear un arreglo dinámico para almacenar los nombres de los recomendados
    std::string* recomendados = new std::string[numRecomendaciones];
    for (int i = 0; i < numRecomendaciones; ++i) {
        recomendados[i] = recomendaciones[i].nodo->nombre;
    }

    cantidadRecomendaciones = numRecomendaciones;

    // Liberar la memoria del arreglo temporal
    delete[] recomendaciones;

    return recomendados;
}

int GrafoNoDirigido::obtenerAmigosEnComun(const std::string& usuario1, const std::string& usuario2) const {
    // Encontrar los nodos correspondientes a usuario1 y usuario2
    Nodo* nodo1 = encontrarNodo(usuario1);
    Nodo* nodo2 = encontrarNodo(usuario2);

    // Si alguno de los usuarios no existe en el grafo, no tienen amigos en común
    if (!nodo1 || !nodo2) {
        return 0;
    }

    // Obtener los amigos (vecinos) de ambos usuarios
    int amigosEnComun = 0;

    for (int i = 0; i < nodo1->numVecinos; ++i) {
        for (int j = 0; j < nodo2->numVecinos; ++j) {
            if (nodo1->vecinos[i]->nombre == nodo2->vecinos[j]->nombre) {
                // Encontramos un amigo en común
                amigosEnComun++;
            }
        }
    }

    return amigosEnComun;
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

void GrafoNoDirigido::generarArchivoDOT(const std::string& nombreArchivo) const {
    std::ofstream archivo(nombreArchivo);
    if (archivo.is_open()) {
        archivo << "digraph G {\n";

        // Arreglo para rastrear los nodos escritos
        std::string nodosEscritos[100]; // Asumimos un máximo de 100 nodos
        int contadorNodos = 0;

        for (int i = 0; i < numNodos; i++) {
            // Verificar si el nodo ya fue escrito
            bool yaEscrito = false;
            for (int k = 0; k < contadorNodos; k++) {
                if (nodosEscritos[k] == nodos[i]->nombre) {
                    yaEscrito = true;
                    break;
                }
            }

            // Si no fue escrito, añadirlo
            if (!yaEscrito) {
                archivo << "\"" << nodos[i]->nombre << "\"[label=\"" << nodos[i]->nombre << "\"];\n";  // Comillas alrededor
                nodosEscritos[contadorNodos++] = nodos[i]->nombre; // Añadir a los nodos escritos
            }

            // Añadir los arcos
            for (int j = 0; j < nodos[i]->numVecinos; j++) {
                if (nodos[i]->nombre < nodos[i]->vecinos[j]->nombre) {  // Evitar duplicados
                    archivo << "\"" << nodos[i]->nombre << "\" -> \"" << nodos[i]->vecinos[j]->nombre << "\"[dir=none];\n";  // Comillas alrededor
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
