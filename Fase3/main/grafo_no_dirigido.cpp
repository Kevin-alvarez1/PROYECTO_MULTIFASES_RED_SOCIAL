#include "grafo_no_dirigido.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <sstream>
GrafoNoDirigido grafoNoDirigido;


const int INICIAL_CAPACIDAD = 5;
#define MAX_ARISTAS 1000

// Constructor de Nodo
Nodo::Nodo(const std::string& nombre) : nombre(nombre), numVecinos(0), capacidadVecinos(INICIAL_CAPACIDAD) {
    vecinos = new Nodo*[capacidadVecinos];  // Crear arreglo dinámico para vecinos
}

// Destructor de Nodo
Nodo::~Nodo() {
    delete[] vecinos;  // Liberar memoria de los vecinos
}

void GrafoNoDirigido::guardarAmigos() const {
    std::ofstream archivoSalida("Amigos.edd");
    if (!archivoSalida.is_open()) {
        std::cerr << "Error al abrir el archivo Amigos.edd" << std::endl;
        return;
    }

    for (int i = 0; i < numNodos; ++i) {
        Nodo* nodo = nodos[i];
        for (int j = 0; j < nodo->numVecinos; ++j) {
            archivoSalida << nodo->nombre << "," << nodo->vecinos[j]->nombre << "," << "ACEPTADA" << std::endl;
        }
    }

    archivoSalida.close(); // Cerrar el archivo
}

void GrafoNoDirigido::cargarAmigos() {
    std::ifstream archivoEntrada("Amigos.edd");
    if (!archivoEntrada.is_open()) {
        std::cerr << "Error al abrir el archivo Amigos.edd" << std::endl;
        return;
    }

    std::string linea;
    while (std::getline(archivoEntrada, linea)) {
        std::istringstream stream(linea);
        std::string emisor, receptor, estado;

        std::getline(stream, emisor, ',');    // Lee el emisor
        std::getline(stream, receptor, ',');   // Lee el receptor
        std::getline(stream, estado, ',');      // Lee el estado

        // Solo insertar la relación si el estado es "ACEPTADA"
        if (estado == "ACEPTADA") {
            insertarNombre(emisor);   // Asegúrate de que el nodo existe
            insertarNombre(receptor);  // Asegúrate de que el nodo existe
            insertarRelacion(emisor, receptor); // Agregar la relación
        }
    }

    archivoEntrada.close(); // Cerrar el archivo
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

void GrafoNoDirigido::eliminarUsuario(const std::string& nombre) {
    // Encontrar el nodo que queremos eliminar
    Nodo* nodoAEliminar = encontrarNodo(nombre);
    if (!nodoAEliminar) {
        std::cerr << "Usuario no encontrado: " << nombre << std::endl;
        return;
    }

    // Eliminar la referencia a este nodo en la lista de vecinos de los demás nodos
    for (int i = 0; i < numNodos; ++i) {
        Nodo* nodoActual = nodos[i];
        if (nodoActual != nodoAEliminar) {
            // Revisar si el nodo actual tiene como vecino al nodo que queremos eliminar
            for (int j = 0; j < nodoActual->numVecinos; ++j) {
                if (nodoActual->vecinos[j] == nodoAEliminar) {
                    // Eliminar la referencia al nodoAEliminar de la lista de vecinos
                    for (int k = j; k < nodoActual->numVecinos - 1; ++k) {
                        nodoActual->vecinos[k] = nodoActual->vecinos[k + 1]; // Desplazar los vecinos hacia la izquierda
                    }
                    nodoActual->numVecinos--;
                    break;
                }
            }
        }
    }

    // Ahora eliminamos el nodoAEliminar del arreglo nodos
    int indexAEliminar = -1;
    for (int i = 0; i < numNodos; ++i) {
        if (nodos[i] == nodoAEliminar) {
            indexAEliminar = i;
            break;
        }
    }

    if (indexAEliminar != -1) {
        // Liberar la memoria del nodo a eliminar
        delete nodoAEliminar;

        // Desplazar los nodos restantes en el arreglo nodos
        for (int i = indexAEliminar; i < numNodos - 1; ++i) {
            nodos[i] = nodos[i + 1];
        }
        numNodos--;
    } else {
        std::cerr << "Error: Nodo a eliminar no encontrado en la lista de nodos." << std::endl;
    }
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

void GrafoNoDirigido::generarArchivoDOTEstilos(const std::string& nombreArchivo, const std::string& correoActualUsuario_) const {
    std::ofstream archivo(nombreArchivo);
    if (archivo.is_open()) {
        archivo << "digraph G {\n";

        // Arreglo para rastrear los nodos escritos
        std::string nodosEscritos[100]; // Asumimos un máximo de 100 nodos
        int contadorNodos = 0;

        // Obtener amigos del usuario actual
        int cantidadAmigos = 0;
        std::string* amigos = obtenerAmigos(correoActualUsuario_, cantidadAmigos);

        // Obtener recomendaciones del usuario actual
        int cantidadRecomendaciones = 0;
        std::string* recomendaciones = recomendarAmigos(correoActualUsuario_, cantidadRecomendaciones);

        // Añadir el nodo del usuario actual
        archivo << "\"" << correoActualUsuario_ << "\" [label=\"" << correoActualUsuario_ << "\" color=\"skyblue\", style=filled];\n";
        nodosEscritos[contadorNodos++] = correoActualUsuario_;

        // Añadir los amigos y sus conexiones
        for (int i = 0; i < cantidadAmigos; i++) {
            if (std::find(nodosEscritos, nodosEscritos + contadorNodos, amigos[i]) == nodosEscritos + contadorNodos) {
                archivo << "\"" << amigos[i] << "\" [label=\"" << amigos[i] << "\" color=\"lightgreen\", style=filled];\n";
                nodosEscritos[contadorNodos++] = amigos[i];
            }
            archivo << "\"" << correoActualUsuario_ << "\" -> \"" << amigos[i] << "\" [dir=none];\n";  // Conexión con amigos
        }

        // Añadir las recomendaciones y sus conexiones
        for (int i = 0; i < cantidadRecomendaciones; i++) {
            if (std::find(nodosEscritos, nodosEscritos + contadorNodos, recomendaciones[i]) == nodosEscritos + contadorNodos) {
                archivo << "\"" << recomendaciones[i] << "\" [label=\"" << recomendaciones[i] << "\" color=\"salmon\", style=filled];\n";
                nodosEscritos[contadorNodos++] = recomendaciones[i];
            }
        }

        // Añadir los nodos restantes del grafo
        for (int i = 0; i < numNodos; i++) {
            // Verificar si el nodo ya fue escrito
            bool yaEscrito = std::find(nodosEscritos, nodosEscritos + contadorNodos, nodos[i]->nombre) != nodosEscritos + contadorNodos;

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

        // Liberar la memoria de las recomendaciones
        delete[] amigos;
        delete[] recomendaciones;
    }
}




void GrafoNoDirigido::generarArchivoDOTListaAdyacencia(const std::string& nombreArchivo) const {
    std::ofstream archivo(nombreArchivo);

    if (archivo.is_open()) {
        archivo << "digraph G {\n";
        archivo << "rankdir=LR;\n";  // Alinea los nodos de izquierda a derecha

        // Definir nodos de manera global para asegurarse de que todos aparecen
        for (int i = 0; i < numNodos; ++i) {
            Nodo* nodoActual = nodos[i];
            archivo << "\"" << nodoActual->nombre << "\" [shape=box];\n";  // Cada nodo como una caja (box)
        }

        // Arreglo para rastrear las aristas escritas
        std::string aristasEscritas[MAX_ARISTAS];
        int contadorAristas = 0;

        // Primera sección: Conexiones de amistad
        archivo << "\n// Conexiones de amistad\n";
        for (int i = 0; i < numNodos; ++i) {
            Nodo* nodoActual = nodos[i];

            // Iterar sobre los vecinos (amistades) de cada nodo
            for (int j = 0; j < nodoActual->numVecinos; ++j) {
                Nodo* vecino = nodoActual->vecinos[j];

                // Crear una representación única de la arista
                std::string nodo1 = nodoActual->nombre;
                std::string nodo2 = vecino->nombre;

                // Ordenar los nodos para evitar duplicados
                std::string arista = (nodo1 < nodo2) ? (nodo1 + "-" + nodo2) : (nodo2 + "-" + nodo1);

                // Verificar si la arista ya fue escrita
                bool yaEscrita = false; // Declarar yaEscrita aquí
                for (int k = 0; k < contadorAristas; k++) {
                    if (aristasEscritas[k] == arista) {
                        yaEscrita = true;
                        break;
                    }
                }

                // Si no fue escrita, añadirla
                if (!yaEscrita) {
                    archivo << "\"" << nodo1 << "\" -> \"" << nodo2 << "\";\n";
                    aristasEscritas[contadorAristas++] = arista;  // Añadir a las aristas escritas
                }
            }
        }

        // Reiniciar el contador y arreglo para las recomendaciones
        contadorAristas = 0;

        // Segunda sección: Recomendaciones
        archivo << "\n// Recomendaciones\n";
        for (int i = 0; i < numNodos; ++i) {
            Nodo* nodoActual = nodos[i];

            // Iterar sobre los vecinos para generar recomendaciones
            for (int j = 0; j < nodoActual->numVecinos; ++j) {
                Nodo* amigo = nodoActual->vecinos[j];

                // Generar recomendaciones basadas en los amigos del amigo
                for (int k = 0; k < amigo->numVecinos; ++k) {
                    Nodo* recomendacion = amigo->vecinos[k];

                    // Evitar recomendar el mismo nodo (nodo actual no puede recomendarse a sí mismo)
                    if (recomendacion != nodoActual) {
                        std::string amigoNombre = amigo->nombre;
                        std::string recomendacionNombre = recomendacion->nombre;

                        // Crear una representación única de la arista de recomendación
                        std::string aristaRecomendacion = (amigoNombre < recomendacionNombre)
                                                              ? (amigoNombre + "-" + recomendacionNombre)
                                                              : (recomendacionNombre + "-" + amigoNombre);

                        // Verificar si la recomendación ya fue escrita
                        bool yaEscrita = false; // Declarar yaEscrita aquí
                        for (int k = 0; k < contadorAristas; k++) {
                            if (aristasEscritas[k] == aristaRecomendacion) {
                                yaEscrita = true;
                                break;
                            }
                        }

                        // Si no fue escrita, añadir la recomendación
                        if (!yaEscrita) {
                            archivo << "\"" << amigo->nombre << "\" -> \"" << recomendacion->nombre << "\" [style=dotted];\n";
                            aristasEscritas[contadorAristas++] = aristaRecomendacion;  // Añadir a las aristas escritas
                        }
                    }
                }
            }
        }

        archivo << "}\n";
        archivo.close();

        std::cout << "Archivo DOT generado correctamente: " << nombreArchivo << std::endl;
    } else {
        std::cerr << "No se pudo abrir el archivo: " << nombreArchivo << std::endl;
    }
}

void GrafoNoDirigido::generarPNG_ListaAdyacencia(const std::string& nombreArchivo) const {
    std::string nombreDOT = nombreArchivo + ".dot";
    std::string nombrePNG = nombreArchivo + ".png";

    generarArchivoDOTListaAdyacencia(nombreDOT);

    // Ejecutar el comando dot para convertir .dot a .png
    std::string comando = "dot -Tpng " + nombreDOT + " -o " + nombrePNG;
    system(comando.c_str());

    std::cout << "Imagen PNG generada correctamente: " << nombrePNG << std::endl;
}

// Generar un PNG del grafo usando Graphviz
void GrafoNoDirigido::generarPNG(const std::string& nombreArchivo) const {
    std::string dotFile = nombreArchivo + ".dot";
    generarArchivoDOT(dotFile);
    std::string comando = "dot -Tpng " + dotFile + " -o " + nombreArchivo + ".png";
    system(comando.c_str());
}
